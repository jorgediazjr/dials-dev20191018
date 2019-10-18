from __future__ import absolute_import, division, print_function

import json
import os
import re
from datetime import datetime

try:  # Python 3
    from urllib.request import urlopen, Request
except ImportError:  # Python 2
    from urllib2 import urlopen, Request


def _download_button(text, version, link):
    print("  %s %s -> %s" % (version, text, link))
    return ".. button::\n   :text: DIALS %s %s\n   :link: %s\n\n" % (
        version,
        text,
        link,
    )


if __name__ == "__main__":
    release_file = os.path.join(
        os.path.dirname(__file__), "installation.stable_release"
    )
    print("Checking DIALS release status")
    url_request = Request("https://api.github.com/repos/dials/dials/releases/latest")
    conn = urlopen(url_request)
    release_info = json.load(conn)
    conn.close()

    with open(release_file, "w") as release:
        caption = "Stable Release"
        if "name" in release_info:
            caption = caption + ": " + release_info["name"]
            print("Most recent major DIALS release is:", release_info["name"])
        else:
            print("Could not determine most recent major DIALS release")
        release.write(caption + "\n" + "=" * len(caption) + "\n\n")

        release.write(
            "The current stable release can be downloaded from `Github <https://github.com/dials/dials/releases/latest>`_,\n"
        )
        release.write(
            "where you can also find further `release notes <https://github.com/dials/dials/releases/latest>`_.\n\n"
        )

        assets = {}
        for a in release_info.get("assets", []):
            tag = re.search("dials-v([^-]+)-([^-]+)-([^-]+)-(.+)", a["name"])
            if tag:
                shortname = tag.group(4)
                version = ".".join(tag.group(1, 2, 3))
                last_update = datetime.strptime(
                    a["updated_at"], "%Y-%m-%dT%H:%M:%SZ"
                )  # - datetime(1970,1,1)).total_seconds()
                if shortname not in assets or assets[shortname][0] < last_update:
                    assets[shortname] = (
                        last_update,
                        version,
                        a.get("browser_download_url"),
                    )

        long_names = {
            "macosx.pkg": "Mac installer",
            "macosx.tar.gz": "Mac tar archive",
            "macosx-10.6.pkg": "Mac installer (OS X 10.6)",
            "macosx-10.6.tar.gz": "Mac tar archive (OS X 10.6)",
            "linux-x86_64.tar.xz": "Linux installer",
            "source.tar.xz": "Source installer",
        }

        buttons = [
            _download_button(long_names.get(asset, asset), _version, link)
            for asset, (_, _version, link) in assets.items()
        ]

        release.write("".join(sorted(buttons)))
