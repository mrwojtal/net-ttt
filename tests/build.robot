*** Settings ***
Library    Process
Library    OperatingSystem

*** Variables ***
${BUILD_DIR}    build

*** Test Cases ***
Project Builds Succesfully
    Run Process    make    clean
    ${result}=    Run Process    make    all
    Should Be Equal As Integers    ${result.rc}    0
    Should Not Contain    ${result.stderr}    warning
    File Should Exist    ${BUILD_DIR}/client_app
    File Should Exist    ${BUILD_DIR}/server_app