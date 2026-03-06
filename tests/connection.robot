*** Settings ***
Library    Process

*** Variables ***
${SERVER}    build/server_app
${CLIENT}    build/client_app

*** Test Cases ***
Test Connection between server and client
    ${server_handle}=    Start Process    ${SERVER}
    Sleep    1s

    ${client_handle}=    Start Process    ${CLIENT}    127.0.0.0
    Sleep    1s

    ${client_result}=    Terminate Process    ${server_handle}
    ${server_result}=    Terminate Process    ${client_handle}

    #client output checking
    Should Contain    ${client_result.stdout}    Server is listening
    Should Contain    ${client_result.stdout}    Server is waiting for connections from clients
    Should Contain    ${client_result.stdout}    Connected with client
    Should Contain    ${client_result.stdout}    Waiting for message from client
    Should Be Empty    ${client_result.stderr}

    #server output checking
    Should Contain    ${server_result.stdout}    Connected with server
    Should Contain    ${server_result.stdout}    Game Menu
    Should Contain    ${server_result.stdout}    User input
    Should Be Empty    ${server_result.stderr}
