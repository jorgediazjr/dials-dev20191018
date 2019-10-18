macro(EXEC_CHECK CMD ARG1 ARG2 ARG3 ARG4 ARG5 ARG6 ARG7 ARG8 ARG9)
    execute_process(COMMAND ${CMD} ${ARG1} ${ARG2} ${ARG3} ${ARG4} ${ARG5} ${ARG6} ${ARG7} ${ARG8} ${ARG9} RESULT_VARIABLE CMD_RESULT)
    if(CMD_RESULT)
        message(FATAL_ERROR "Error running ${CMD} ${ARG1} ${ARG2} ${ARG3} ${ARG4} ${ARG5} ${ARG6} ${ARG7} ${ARG8} ${ARG9}")
    endif()
endmacro()
if(CMD1)
exec_check(${CMD1} "${ARG11}" "${ARG12}" "${ARG13}" "${ARG14}" "${ARG15}" "${ARG16}" "${ARG17}" "${ARG18}" "${ARG19}")
endif()
if(CMD2)
exec_check(${CMD2} "${ARG21}" "${ARG22}" "${ARG23}" "${ARG24}" "${ARG25}" "${ARG26}" "${ARG27}" "${ARG28}" "${ARG29}")
endif()
if(CMD3)
exec_check(${CMD3} "${ARG31}" "${ARG32}" "${ARG33}" "${ARG34}" "${ARG35}" "${ARG36}" "${ARG37}" "${ARG38}" "${ARG39}")
endif()
if(CMD4)
exec_check(${CMD4} "${ARG41}" "${ARG42}" "${ARG43}" "${ARG44}" "${ARG45}" "${ARG46}" "${ARG47}" "${ARG48}" "${ARG49}")
endif()
if(CMD5)
exec_check(${CMD5} "${ARG51}" "${ARG52}" "${ARG53}" "${ARG54}" "${ARG55}" "${ARG56}" "${ARG57}" "${ARG58}" "${ARG59}")
endif()
if(CMD6)
exec_check(${CMD6} "${ARG61}" "${ARG62}" "${ARG63}" "${ARG64}" "${ARG65}" "${ARG66}" "${ARG67}" "${ARG68}" "${ARG69}")
endif()
if(CMD7)
exec_check(${CMD7} "${ARG71}" "${ARG72}" "${ARG73}" "${ARG74}" "${ARG75}" "${ARG76}" "${ARG77}" "${ARG78}" "${ARG79}")
endif()
if(CMD8)
exec_check(${CMD8} "${ARG81}" "${ARG82}" "${ARG83}" "${ARG84}" "${ARG85}" "${ARG86}" "${ARG87}" "${ARG88}" "${ARG89}")
endif()
if(CMD9)
exec_check(${CMD9} "${ARG91}" "${ARG92}" "${ARG93}" "${ARG94}" "${ARG95}" "${ARG96}" "${ARG97}" "${ARG98}" "${ARG99}")
endif()
if(CMD10)
exec_check(${CMD10} "${ARG101}" "${ARG102}" "${ARG103}" "${ARG104}" "${ARG105}" "${ARG106}" "${ARG107}" "${ARG108}" "${ARG109}")
endif()
if(CMD11)
exec_check(${CMD11} "${ARG111}" "${ARG112}" "${ARG113}" "${ARG114}" "${ARG115}" "${ARG116}" "${ARG117}" "${ARG118}" "${ARG119}")
endif()
if(CMD12)
exec_check(${CMD12} "${ARG121}" "${ARG122}" "${ARG123}" "${ARG124}" "${ARG125}" "${ARG126}" "${ARG127}" "${ARG128}" "${ARG129}")
endif()
