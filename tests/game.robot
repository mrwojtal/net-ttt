*** Settings ***
Library    Process

*** Variables ***
${SERVER}    build/server_app
${CLIENT}    build/client_app
#game scenario variables
${SERVER_STDIN}    ${None}
${CLIENT_STDIN}    ${None}

*** Test Cases ***
Test server winning scenario
    Set Test Variable    ${SERVER_STDIN}    0\n1\n2\n
    Set Test Variable    ${CLIENT_STDIN}    2\n3\n4\n3\n

    ${server_handle}=    Start Process    ${SERVER}    stdin=${SERVER_STDIN}
    Sleep    1s

    ${client_handle}=    Start Process    ${CLIENT}    127.0.0.0    stdin=${CLIENT_STDIN}
    
    ${client_result}=    Wait For Process    ${client_handle}    5s
    ${server_result}=    Terminate Process    ${server_handle}

    #client output checking
    Should Contain    ${client_result.stdout}    Connected with server
    Should Contain    ${client_result.stdout}    Game Menu
    Should Contain    ${client_result.stdout}    User input
    Should Contain    ${client_result.stdout}    Wait for opponent
    Should Contain    ${client_result.stdout}    Your move
    Should Contain    ${client_result.stdout}    You lose
    Should Contain    ${client_result.stdout}    Exitting
    Should Be Empty    ${client_result.stderr}

    #server output checking
    Should Contain    ${server_result.stdout}    Server is listening
    Should Contain    ${server_result.stdout}    Server is waiting for connections from clients
    Should Contain    ${server_result.stdout}    Connected with client
    Should Contain    ${server_result.stdout}    Waiting for message from client
    Should Contain    ${server_result.stdout}    User input
    Should Contain    ${server_result.stdout}    Wait for opponent
    Should Contain    ${server_result.stdout}    Your move
    Should Contain    ${server_result.stdout}    You win
    Should Contain    ${server_result.stderr}    disconnected

Test client winning scenario
    Set Test Variable    ${SERVER_STDIN}    0\n1\n6\n
    Set Test Variable    ${CLIENT_STDIN}    2\n3\n4\n5\n3\n

    ${server_handle}=    Start Process    ${SERVER}    stdin=${SERVER_STDIN}
    Sleep    1s

    ${client_handle}=    Start Process    ${CLIENT}    127.0.0.0    stdin=${CLIENT_STDIN}
    
    ${client_result}=    Wait For Process    ${client_handle}    5s
    ${server_result}=    Terminate Process    ${server_handle}

    #client output checking
    Should Contain    ${client_result.stdout}    Connected with server
    Should Contain    ${client_result.stdout}    Game Menu
    Should Contain    ${client_result.stdout}    User input
    Should Contain    ${client_result.stdout}    Wait for opponent
    Should Contain    ${client_result.stdout}    Your move
    Should Contain    ${client_result.stdout}    You win
    Should Contain    ${client_result.stdout}    Exitting
    Should Be Empty    ${client_result.stderr}

    #server output checking
    Should Contain    ${server_result.stdout}    Server is listening
    Should Contain    ${server_result.stdout}    Server is waiting for connections from clients
    Should Contain    ${server_result.stdout}    Connected with client
    Should Contain    ${server_result.stdout}    Waiting for message from client
    Should Contain    ${server_result.stdout}    User input
    Should Contain    ${server_result.stdout}    Wait for opponent
    Should Contain    ${server_result.stdout}    Your move
    Should Contain    ${server_result.stdout}    You lose
    Should Contain    ${server_result.stderr}    disconnected

Test draw scenario
    Set Test Variable    ${SERVER_STDIN}    0\n1\n4\n5\n6\n
    Set Test Variable    ${CLIENT_STDIN}    2\n3\n2\n8\n7\n3\n

    ${server_handle}=    Start Process    ${SERVER}    stdin=${SERVER_STDIN}
    Sleep    1s

    ${client_handle}=    Start Process    ${CLIENT}    127.0.0.0    stdin=${CLIENT_STDIN}
    
    ${client_result}=    Wait For Process    ${client_handle}    5s
    ${server_result}=    Terminate Process    ${server_handle}

    #client output checking
    Should Contain    ${client_result.stdout}    Connected with server
    Should Contain    ${client_result.stdout}    Game Menu
    Should Contain    ${client_result.stdout}    User input
    Should Contain    ${client_result.stdout}    Wait for opponent
    Should Contain    ${client_result.stdout}    Your move
    Should Contain    ${client_result.stdout}    Draw
    Should Contain    ${client_result.stdout}    Exitting
    Should Be Empty    ${client_result.stderr}

    #server output checking
    Should Contain    ${server_result.stdout}    Server is listening
    Should Contain    ${server_result.stdout}    Server is waiting for connections from clients
    Should Contain    ${server_result.stdout}    Connected with client
    Should Contain    ${server_result.stdout}    Waiting for message from client
    Should Contain    ${server_result.stdout}    User input
    Should Contain    ${server_result.stdout}    Wait for opponent
    Should Contain    ${server_result.stdout}    Your move
    Should Contain    ${server_result.stdout}    Draw
    Should Contain    ${server_result.stderr}    disconnected