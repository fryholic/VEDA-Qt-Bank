import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 600
    title: "한화 가상 은행"
    
    // 색상 정의
    property color hanwhaOrange100: "#F3732B"
    property color hanwhaOrange70: "#F89B6C"
    property color hanwhaOrange50: "#FBB584"
    property color hanwhaBlack: "#000000"
    property color backgroundColor: "#F8F9FA"
    property color textSecondary: "#495057"
    property color successColor: "#28A745"
    property color warningColor: "#FFC107"
    property color errorColor: "#DC3545"
    property color linkColor: "#0056B3"
    
    // 현재 화면 상태
    property string currentScreen: "login"
    property string previousScreen: ""
    
    // 현재 선택된 계좌
    property string selectedAccount: ""
    
    // 거래 관련 임시 데이터
    property double transactionAmount: 0
    property string transactionType: ""
    property string targetAccount: ""
    
    // 전역 폰트 설정
    font.family: "Noto Sans KR"
    
    // 배경 색상 설정
    Rectangle {
        anchors.fill: parent
        color: backgroundColor
    }
    
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: loginScreen
    }
    
    // 로그인 화면
    Component {
        id: loginScreen
        
        Rectangle {
            color: backgroundColor
            
            ColumnLayout {
                anchors.centerIn: parent
                width: 300
                spacing: 20
                
                Image {
                    Layout.alignment: Qt.AlignHCenter
                    source: "qrc:/images/bank_logo.png"
                    width: 150
                    height: 150
                    fillMode: Image.PreserveAspectFit
                }
                
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "한화 가상 은행"
                    font.pixelSize: 24
                    font.bold: true
                    color: hanwhaBlack
                }
                
                TextField {
                    id: usernameField
                    Layout.fillWidth: true
                    placeholderText: "아이디"
                    font.pixelSize: 16
                    background: Rectangle {
                        color: "white"
                        border.color: usernameField.focus ? hanwhaOrange100 : "#CCCCCC"
                        border.width: 1
                        radius: 4
                    }
                }
                
                TextField {
                    id: passwordField
                    Layout.fillWidth: true
                    placeholderText: "비밀번호"
                    echoMode: TextInput.Password
                    font.pixelSize: 16
                    background: Rectangle {
                        color: "white"
                        border.color: passwordField.focus ? hanwhaOrange100 : "#CCCCCC"
                        border.width: 1
                        radius: 4
                    }
                }
                
                Button {
                    Layout.fillWidth: true
                    text: "로그인"
                    font.pixelSize: 16
                    
                    background: Rectangle {
                        color: parent.down ? hanwhaOrange50 : (parent.hovered ? hanwhaOrange70 : hanwhaOrange100)
                        radius: 4
                        
                        // 그림자 효과
                        layer.enabled: true
                        layer.effect: MultiEffect {
                            shadowEnabled: true
                            shadowColor: Qt.rgba(0, 0, 0, 0.2)
                            shadowHorizontalOffset: 0
                            shadowVerticalOffset: 2
                            shadowBlur: 4
                        }
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    
                    onClicked: {
                        if (bankModel.login(usernameField.text, passwordField.text)) {
                            currentScreen = "main"
                            stackView.push(mainScreen)
                        } else {
                            loginErrorDialog.open()
                        }
                    }
                }
                
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "회원가입"
                    font.pixelSize: 14
                    color: linkColor
                    
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            // 회원가입 화면으로 이동 (실제 구현은 생략)
                        }
                    }
                }
            }
        }
    }
    
    // 메인 화면
    Component {
        id: mainScreen
        
        Rectangle {
            color: backgroundColor
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20
                
                // 상단 헤더
                Rectangle {
                    Layout.fillWidth: true
                    height: 80
                    color: hanwhaOrange100
                    radius: 8
                    
                    // 그림자 효과
                    layer.enabled: true
                    layer.effect: MultiEffect {
                        shadowEnabled: true
                        shadowColor: Qt.rgba(0, 0, 0, 0.2)
                        shadowHorizontalOffset: 0
                        shadowVerticalOffset: 2
                        shadowBlur: 4
                    }
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        
                        Column {
                            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                            
                            Text {
                                text: bankModel.userName + " 님, 안녕하세요!"
                                font.pixelSize: 18
                                font.bold: true
                                color: "white"
                            }
                            
                            Text {
                                text: bankModel.userName + " 님"
                                font.pixelSize: 14
                                color: "white"
                            }
                        }
                        
                        Item { Layout.fillWidth: true }
                        
                        Column {
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            
                            Text {
                                text: "총 계좌 금액"
                                font.pixelSize: 14
                                color: "white"
                                anchors.right: parent.right
                            }
                            
                            Text {
                                text: bankModel.totalBalance.toLocaleString() + " 원"
                                font.pixelSize: 18
                                font.bold: true
                                color: "white"
                                anchors.right: parent.right
                            }
                        }
                    }
                }
                
                // 계좌 목록 타이틀
                RowLayout {
                    Layout.fillWidth: true
                    
                    Text {
                        text: "전체 계좌 리스트"
                        font.pixelSize: 18
                        font.bold: true
                        color: hanwhaBlack
                    }
                    
                    Item { Layout.fillWidth: true }
                    
                    Button {
                        text: "계좌 추가"
                        font.pixelSize: 14
                        
                        background: Rectangle {
                            color: parent.down ? "#E9ECEF" : (parent.hovered ? "#DEE2E6" : "#F8F9FA")
                            border.color: "#CED4DA"
                            border.width: 1
                            radius: 4
                        }
                        
                        contentItem: Text {
                            text: parent.text
                            font: parent.font
                            color: textSecondary
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        
                        onClicked: {
                            // 계좌 추가 기능 (실제 구현은 생략)
                        }
                    }
                }
                
                // 계좌 목록
                ListView {
                    id: accountListView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: bankModel.getAccounts()
                    spacing: 10
                    
                    delegate: Rectangle {
                        width: accountListView.width
                        height: 80
                        color: "white"
                        border.color: "#EEEEEE"
                        border.width: 1
                        radius: 8
                        
                        // 그림자 효과
                        layer.enabled: true
                        layer.effect: MultiEffect {
                            shadowEnabled: true
                            shadowColor: Qt.rgba(0, 0, 0, 0.1)
                            shadowHorizontalOffset: 0
                            shadowVerticalOffset: 1
                            shadowBlur: 3
                        }
                        
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                selectedAccount = modelData.accountNumber
                                bankModel.setCurrentAccountNumber(selectedAccount)
                                currentScreen = "account"
                                stackView.push(accountScreen)
                            }
                        }
                        
                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 16
                            
                            Rectangle {
                                width: 40
                                height: 40
                                radius: 20
                                color: hanwhaOrange50
                                
                                Text {
                                    anchors.centerIn: parent
                                    text: modelData.accountName.charAt(0)
                                    font.pixelSize: 18
                                    font.bold: true
                                    color: hanwhaOrange100
                                }
                            }
                            
                            Column {
                                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                                Layout.leftMargin: 10
                                
                                Text {
                                    text: modelData.accountName
                                    font.pixelSize: 16
                                    font.bold: true
                                    color: hanwhaBlack
                                }
                                
                                Text {
                                    text: modelData.accountNumber
                                    font.pixelSize: 14
                                    color: textSecondary
                                }
                            }
                            
                            Item { Layout.fillWidth: true }
                            
                            Column {
                                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                                
                                Text {
                                    text: modelData.balance.toLocaleString() + " 원"
                                    font.pixelSize: 16
                                    font.bold: true
                                    color: hanwhaOrange100
                                    anchors.right: parent.right
                                }
                                
                                Text {
                                    text: "잔액"
                                    font.pixelSize: 12
                                    color: textSecondary
                                    anchors.right: parent.right
                                }
                            }
                            
                            Image {
                                source: "qrc:/images/arrow_right.png"
                                width: 24
                                height: 24
                            }
                        }
                    }
                }
                
                // 로그아웃 버튼
                Button {
                    Layout.alignment: Qt.AlignRight
                    text: "로그아웃"
                    font.pixelSize: 12
                    
                    background: Rectangle {
                        color: parent.down ? "#EEEEEE" : (parent.hovered ? "#F5F5F5" : "white")
                        border.color: "#CCCCCC"
                        border.width: 1
                        radius: 4
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: textSecondary
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    
                    onClicked: {
                        bankModel.logout()
                        currentScreen = "login"
                        stackView.pop(null)
                    }
                }
            }
        }
    }
    
    // 계좌 관리 화면
    Component {
        id: accountScreen
        
        Rectangle {
            color: backgroundColor
            
            property var accountDetails: bankModel.getAccountDetails(selectedAccount)
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20
                
                // 상단 헤더
                Rectangle {
                    Layout.fillWidth: true
                    height: 60
                    color: "white"
                    radius: 8
                    
                    // 그림자 효과
                    layer.enabled: true
                    layer.effect: MultiEffect {
                        shadowEnabled: true
                        shadowColor: Qt.rgba(0, 0, 0, 0.1)
                        shadowHorizontalOffset: 0
                        shadowVerticalOffset: 1
                        shadowBlur: 3
                    }
                    
                    RowLayout {
                        anchors.fill: parent
                        
                        Button {
                            text: "뒤로"
                            font.pixelSize: 14
                            
                            background: Rectangle {
                                color: parent.down ? "#EEEEEE" : (parent.hovered ? "#F5F5F5" : "white")
                                border.color: "#CCCCCC"
                                border.width: 1
                                radius: 4
                            }
                            
                            contentItem: Text {
                                text: parent.text
                                font: parent.font
                                color: textSecondary
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            
                            onClicked: {
                                currentScreen = "main"
                                stackView.pop()
                            }
                        }
                        
                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            text: "계좌 관리"
                            font.pixelSize: 18
                            font.bold: true
                            color: hanwhaBlack
                        }
                        
                        Item { Layout.fillWidth: true }
                    }
                }
                
                // 계좌 정보
                Rectangle {
                    Layout.fillWidth: true
                    height: 120
                    color: hanwhaOrange50
                    radius: 8
                    
                    // 그림자 효과
                    layer.enabled: true
                    layer.effect: MultiEffect {
                        shadowEnabled: true
                        shadowColor: Qt.rgba(0, 0, 0, 0.1)
                        shadowHorizontalOffset: 0
                        shadowVerticalOffset: 2
                        shadowBlur: 4
                    }
                    
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 8
                        
                        Text {
                            text: "세부 계좌 정보"
                            font.pixelSize: 14
                            color: hanwhaBlack
                        }
                        
                        Text {
                            text: accountDetails.accountName
                            font.pixelSize: 18
                            font.bold: true
                            color: hanwhaBlack
                        }
                        
                        Text {
                            text: accountDetails.accountNumber
                            font.pixelSize: 14
                            color: hanwhaBlack
                        }
                        
                        Text {
                            text: accountDetails.balance.toLocaleString() + " 원"
                            font.pixelSize: 20
                            font.bold: true
                            color: hanwhaBlack
                        }
                    }
                }
                
                // 거래 버튼
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    
                    Button {
                        Layout.fillWidth: true
                        text: "입금"
                        font.pixelSize: 16
                        
                        background: Rectangle {
                            color: parent.down ? hanwhaOrange50 : (parent.hovered ? hanwhaOrange70 : hanwhaOrange100)
                            radius: 4
                            
                            // 그림자 효과
                            layer.enabled: true
                            layer.effect: MultiEffect {
                                shadowEnabled: true
                                shadowColor: Qt.rgba(0, 0, 0, 0.2)
                                shadowHorizontalOffset: 0
                                shadowVerticalOffset: 2
                                shadowBlur: 4
                            }
                        }
                        
                        contentItem: Text {
                            text: parent.text
                            font: parent.font
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        
                        onClicked: {
                            transactionType = "deposit"
                            currentScreen = "amount"
                            stackView.push(amountScreen)
                        }
                    }
                    
                    Button {
                        Layout.fillWidth: true
                        text: "출금"
                        font.pixelSize: 16
                        
                        background: Rectangle {
                            color: parent.down ? hanwhaOrange50 : (parent.hovered ? hanwhaOrange70 : hanwhaOrange100)
                            radius: 4
                            
                            // 그림자 효과
                            layer.enabled: true
                            layer.effect: MultiEffect {
                                shadowEnabled: true
                                shadowColor: Qt.rgba(0, 0, 0, 0.2)
                                shadowHorizontalOffset: 0
                                shadowVerticalOffset: 2
                                shadowBlur: 4
                            }
                        }
                        
                        contentItem: Text {
                            text: parent.text
                            font: parent.font
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        
                        onClicked: {
                            transactionType = "withdraw"
                            currentScreen = "amount"
                            stackView.push(amountScreen)
                        }
                    }
                    
                    Button {
                        Layout.fillWidth: true
                        text: "송금"
                        font.pixelSize: 16
                        
                        background: Rectangle {
                            color: parent.down ? hanwhaOrange50 : (parent.hovered ? hanwhaOrange70 : hanwhaOrange100)
                            radius: 4
                            
                            // 그림자 효과
                            layer.enabled: true
                            layer.effect: MultiEffect {
                                shadowEnabled: true
                                shadowColor: Qt.rgba(0, 0, 0, 0.2)
                                shadowHorizontalOffset: 0
                                shadowVerticalOffset: 2
                                shadowBlur: 4
                            }
                        }
                        
                        contentItem: Text {
                            text: parent.text
                            font: parent.font
                            color: "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        
                        onClicked: {
                            transactionType = "transfer"
                            currentScreen = "amount"
                            stackView.push(amountScreen)
                        }
                    }
                }
                
                // 거래 내역 타이틀
                Text {
                    text: "입출금 내역"
                    font.pixelSize: 18
                    font.bold: true
                    color: hanwhaBlack
                }
                
                // 거래 내역
                ListView {
                    id: transactionListView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: bankModel.getTransactions(selectedAccount)
                    spacing: 8
                    
                    delegate: Rectangle {
                        width: transactionListView.width
                        height: 70
                        color: "white"
                        border.color: "#EEEEEE"
                        border.width: 1
                        radius: 8
                        
                        // 그림자 효과
                        layer.enabled: true
                        layer.effect: MultiEffect {
                            shadowEnabled: true
                            shadowColor: Qt.rgba(0, 0, 0, 0.1)
                            shadowHorizontalOffset: 0
                            shadowVerticalOffset: 1
                            shadowBlur: 2
                        }
                        
                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 16
                            
                            Rectangle {
                                width: 36
                                height: 36
                                radius: 18
                                color: {
                                    if (modelData.type === "입금") return "#E8F5E9"
                                    else if (modelData.type === "출금") return "#FFEBEE"
                                    else return "#E3F2FD"
                                }
                                
                                Text {
                                    anchors.centerIn: parent
                                    text: {
                                        if (modelData.type === "입금") return "+"
                                        else if (modelData.type === "출금") return "-"
                                        else return "→"
                                    }
                                    font.pixelSize: 18
                                    font.bold: true
                                    color: {
                                        if (modelData.type === "입금") return successColor
                                        else if (modelData.type === "출금") return errorColor
                                        else return linkColor
                                    }
                                }
                            }
                            
                            Column {
                                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                                Layout.leftMargin: 10
                                
                                Text {
                                    text: modelData.description
                                    font.pixelSize: 16
                                    font.bold: true
                                    color: hanwhaBlack
                                }
                                
                                Text {
                                    text: modelData.date
                                    font.pixelSize: 14
                                    color: textSecondary
                                }
                            }
                            
                            Item { Layout.fillWidth: true }
                            
                            Text {
                                text: (modelData.type === "출금" || modelData.type === "송금" ? "-" : "+") + 
                                      modelData.amount.toLocaleString() + " 원"
                                font.pixelSize: 16
                                font.bold: true
                                color: modelData.type === "출금" || modelData.type === "송금" ? errorColor : successColor
                            }
                        }
                    }
                    
                    // 거래 내역이 없을 때 표시할 메시지
                    Rectangle {
                        anchors.centerIn: parent
                        width: parent.width * 0.8
                        height: 100
                        color: "white"
                        border.color: "#EEEEEE"
                        border.width: 1
                        radius: 8
                        visible: transactionListView.count === 0
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 10
                            
                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: "거래 내역이 없습니다"
                                font.pixelSize: 16
                                color: textSecondary
                            }
                            
                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: "첫 거래를 시작해보세요!"
                                font.pixelSize: 14
                                color: textSecondary
                            }
                        }
                    }
                }
            }
        }
    }
    
    // 금액 입력 화면
    Component {
        id: amountScreen
        
        Rectangle {
            color: backgroundColor
            
            property var accountDetails: bankModel.getAccountDetails(selectedAccount)
            property string screenTitle: transactionType === "deposit" ? "입금 금액 입력" : 
                                        (transactionType === "withdraw" ? "출금 금액 입력" : "송금 금액 입력")
            property string limitText: transactionType === "deposit" ? 
                                      "0원 초과 ~ 4,200,000,000원 이하로 입력해주세요" : 
                                      "0원 초과 ~ " + accountDetails.balance.toLocaleString() + "원 이하로 입력해주세요"
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20
                
                // 상단 헤더
                Rectangle {
                    Layout.fillWidth: true
                    height: 60
                    color: "white"
                    radius: 8
                    
                    // 그림자 효과
                    layer.enabled: true
                    layer.effect: MultiEffect {
                        shadowEnabled: true
                        shadowColor: Qt.rgba(0, 0, 0, 0.1)
                        shadowHorizontalOffset: 0
                        shadowVerticalOffset: 1
                        shadowBlur: 3
                    }
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        
                        Button {
                            text: "뒤로"
                            font.pixelSize: 14
                            
                            background: Rectangle {
                                color: parent.down ? "#EEEEEE" : (parent.hovered ? "#F5F5F5" : "white")
                                border.color: "#CCCCCC"
                                border.width: 1
                                radius: 4
                            }
                            
                            contentItem: Text {
                                text: parent.text
                                font: parent.font
                                color: textSecondary
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            
                            onClicked: {
                                currentScreen = "account"
                                stackView.pop()
                            }
                        }
                        
                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            text: screenTitle
                            font.pixelSize: 18
                            font.bold: true
                            color: hanwhaBlack
                        }
                        
                        Item { Layout.fillWidth: true }
                    }
                }
                
                // 금액 입력 영역
                Rectangle {
                    Layout.fillWidth: true
                    color: "white"
                    radius: 8
                    height: 200
                    
                    // 그림자 효과
                    layer.enabled: true
                    layer.effect: MultiEffect {
                        shadowEnabled: true
                        shadowColor: Qt.rgba(0, 0, 0, 0.1)
                        shadowHorizontalOffset: 0
                        shadowVerticalOffset: 2
                        shadowBlur: 4
                    }
                    
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 10
                        
                        Text {
                            text: "금액 입력"
                            font.pixelSize: 16
                            font.bold: true
                            color: hanwhaBlack
                        }
                        
                        TextField {
                            id: amountField
                            Layout.fillWidth: true
                            placeholderText: "금액을 입력하세요"
                            font.pixelSize: 16
                            inputMethodHints: Qt.ImhDigitsOnly
                            validator: DoubleValidator {
                                bottom: 0
                                top: 4200000000
                                notation: DoubleValidator.StandardNotation
                            }
                            
                            background: Rectangle {
                                color: "white"
                                border.color: amountField.focus ? hanwhaOrange100 : "#CCCCCC"
                                border.width: 1
                                radius: 4
                            }
                            
                            onTextChanged: {
                                var amount = parseFloat(text)
                                if (isNaN(amount) || amount <= 0) {
                                    amountErrorText.text = "0원 초과의 금액을 입력해주세요"
                                    amountErrorText.visible = true
                                } else if (transactionType !== "deposit" && amount > accountDetails.balance) {
                                    amountErrorText.text = accountDetails.balance.toLocaleString() + "원 이하로 입력해주세요"
                                    amountErrorText.visible = true
                                } else if (amount > 4200000000) {
                                    amountErrorText.text = "4,200,000,000원 이하로 입력해주세요"
                                    amountErrorText.visible = true
                                } else {
                                    amountErrorText.visible = false
                                }
                            }
                        }
                        
                        Text {
                            id: amountErrorText
                            Layout.fillWidth: true
                            text: limitText
                            font.pixelSize: 14
                            color: errorColor
                            visible: false
                        }
                        
                        Rectangle {
                            Layout.fillWidth: true
                            height: 40
                            color: "#333333"
                            radius: 4
                            
                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 8
                                
                                Image {
                                    source: "qrc:/images/info_icon.png"
                                    width: 24
                                    height: 24
                                }
                                
                                Text {
                                    Layout.fillWidth: true
                                    text: limitText
                                    font.pixelSize: 14
                                    color: "white"
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                        
                        // 현재 계좌 잔액 표시 (출금/송금 시)
                        Rectangle {
                            Layout.fillWidth: true
                            height: 40
                            color: "#F8F9FA"
                            radius: 4
                            visible: transactionType === "withdraw" || transactionType === "transfer"
                            
                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 8
                                
                                Text {
                                    text: "현재 계좌 잔액:"
                                    font.pixelSize: 14
                                    color: textSecondary
                                }
                                
                                Item { Layout.fillWidth: true }
                                
                                Text {
                                    text: accountDetails.balance.toLocaleString() + " 원"
                                    font.pixelSize: 14
                                    font.bold: true
                                    color: hanwhaBlack
                                }
                            }
                        }
                    }
                }
                
                Item { Layout.fillHeight: true }
                
                // 다음 버튼
                Button {
                    Layout.fillWidth: true
                    text: "다음"
                    font.pixelSize: 16
                    enabled: {
                        var amount = parseFloat(amountField.text)
                        return !isNaN(amount) && amount > 0 && 
                               (transactionType === "deposit" || amount <= accountDetails.balance) && 
                               amount <= 4200000000
                    }
                    
                    background: Rectangle {
                        color: parent.enabled ? 
                               (parent.down ? hanwhaOrange50 : (parent.hovered ? hanwhaOrange70 : hanwhaOrange100)) : 
                               "#CCCCCC"
                        radius: 4
                        
                        // 그림자 효과
                        layer.enabled: true
                        layer.effect: MultiEffect {
                            shadowEnabled: true
                            shadowColor: Qt.rgba(0, 0, 0, 0.2)
                            shadowHorizontalOffset: 0
                            shadowVerticalOffset: 2
                            shadowBlur: 4
                        }
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    
                    onClicked: {
                        transactionAmount = parseFloat(amountField.text)
                        if (transactionType === "transfer") {
                            currentScreen = "accountNumber"
                            stackView.push(accountNumberScreen)
                        } else {
                            currentScreen = "verification"
                            stackView.push(verificationScreen)
                        }
                    }
                }
            }
        }
    }
    
    // 계좌번호 입력 화면 (송금용)
    Component {
        id: accountNumberScreen
        
        Rectangle {
            color: backgroundColor
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20
                
                // 상단 헤더
                Rectangle {
                    Layout.fillWidth: true
                    height: 60
                    color: "white"
                    radius: 8
                    
                    // 그림자 효과
                    layer.enabled: true
                    layer.effect: MultiEffect {
                        shadowEnabled: true
                        shadowColor: Qt.rgba(0, 0, 0, 0.1)
                        shadowHorizontalOffset: 0
                        shadowVerticalOffset: 1
                        shadowBlur: 3
                    }
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        
                        Button {
                            text: "뒤로"
                            font.pixelSize: 14
                            
                            background: Rectangle {
                                color: parent.down ? "#EEEEEE" : (parent.hovered ? "#F5F5F5" : "white")
                                border.color: "#CCCCCC"
                                border.width: 1
                                radius: 4
                            }
                            
                            contentItem: Text {
                                text: parent.text
                                font: parent.font
                                color: textSecondary
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            
                            onClicked: {
                                currentScreen = "amount"
                                stackView.pop()
                            }
                        }
                        
                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            text: "계좌번호 입력"
                            font.pixelSize: 18
                            font.bold: true
                            color: hanwhaBlack
                        }
                        
                        Item { Layout.fillWidth: true }
                    }
                }
                
                // 계좌번호 입력 영역
                Rectangle {
                    Layout.fillWidth: true
                    color: "white"
                    radius: 8
                    height: 150
                    
                    // 그림자 효과
                    layer.enabled: true
                    layer.effect: MultiEffect {
                        shadowEnabled: true
                        shadowColor: Qt.rgba(0, 0, 0, 0.1)
                        shadowHorizontalOffset: 0
                        shadowVerticalOffset: 2
                        shadowBlur: 4
                    }
                    
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 10
                        
                        Text {
                            text: "송금할 계좌번호"
                            font.pixelSize: 16
                            font.bold: true
                            color: hanwhaBlack
                        }
                        
                        TextField {
                            id: accountNumberField
                            Layout.fillWidth: true
                            placeholderText: "계좌번호를 입력하세요"
                            font.pixelSize: 16
                            
                            background: Rectangle {
                                color: "white"
                                border.color: accountNumberField.focus ? hanwhaOrange100 : "#CCCCCC"
                                border.width: 1
                                radius: 4
                            }
                        }
                        
                        Text {
                            id: accountNumberErrorText
                            Layout.fillWidth: true
                            text: "존재하지 않는 계좌번호입니다"
                            font.pixelSize: 14
                            color: errorColor
                            visible: false
                        }
                        
                        Text {
                            text: "예시: 1234-5678-9012"
                            font.pixelSize: 12
                            color: textSecondary
                        }
                    }
                }
                
                Item { Layout.fillHeight: true }
                
                // 다음 버튼
                Button {
                    Layout.fillWidth: true
                    text: "다음"
                    font.pixelSize: 16
                    enabled: accountNumberField.text.length > 0
                    
                    background: Rectangle {
                        color: parent.enabled ? 
                               (parent.down ? hanwhaOrange50 : (parent.hovered ? hanwhaOrange70 : hanwhaOrange100)) : 
                               "#CCCCCC"
                        radius: 4
                        
                        // 그림자 효과
                        layer.enabled: true
                        layer.effect: MultiEffect {
                            shadowEnabled: true
                            shadowColor: Qt.rgba(0, 0, 0, 0.2)
                            shadowHorizontalOffset: 0
                            shadowVerticalOffset: 2
                            shadowBlur: 4
                        }
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    
                    onClicked: {
                        // 실제로는 계좌번호 유효성 검사를 해야 함
                        // 여기서는 간단히 처리
                        var accounts = bankModel.getAccounts()
                        var validAccount = false
                        
                        for (var i = 0; i < accounts.length; i++) {
                            if (accounts[i].accountNumber === accountNumberField.text) {
                                validAccount = true
                                break
                            }
                        }
                        
                        if (validAccount && accountNumberField.text !== selectedAccount) {
                            targetAccount = accountNumberField.text
                            currentScreen = "verification"
                            stackView.push(verificationScreen)
                        } else {
                            accountNumberErrorText.visible = true
                        }
                    }
                }
            }
        }
    }
    
    // 인증번호 입력 화면
    Component {
        id: verificationScreen
        
        Rectangle {
            color: backgroundColor
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20
                
                // 상단 헤더
                Rectangle {
                    Layout.fillWidth: true
                    height: 60
                    color: "white"
                    radius: 8
                    
                    // 그림자 효과
                    layer.enabled: true
                    layer.effect: MultiEffect {
                        shadowEnabled: true
                        shadowColor: Qt.rgba(0, 0, 0, 0.1)
                        shadowHorizontalOffset: 0
                        shadowVerticalOffset: 1
                        shadowBlur: 3
                    }
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        
                        Button {
                            text: "뒤로"
                            font.pixelSize: 14
                            
                            background: Rectangle {
                                color: parent.down ? "#EEEEEE" : (parent.hovered ? "#F5F5F5" : "white")
                                border.color: "#CCCCCC"
                                border.width: 1
                                radius: 4
                            }
                            
                            contentItem: Text {
                                text: parent.text
                                font: parent.font
                                color: textSecondary
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            
                            onClicked: {
                                currentScreen = transactionType === "transfer" ? "accountNumber" : "amount"
                                stackView.pop()
                            }
                        }
                        
                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            text: "인증번호 입력"
                            font.pixelSize: 18
                            font.bold: true
                            color: hanwhaBlack
                        }
                        
                        Item { Layout.fillWidth: true }
                    }
                }
                
                // 인증번호 입력 영역
                Rectangle {
                    Layout.fillWidth: true
                    color: "white"
                    radius: 8
                    height: 150
                    
                    // 그림자 효과
                    layer.enabled: true
                    layer.effect: MultiEffect {
                        shadowEnabled: true
                        shadowColor: Qt.rgba(0, 0, 0, 0.1)
                        shadowHorizontalOffset: 0
                        shadowVerticalOffset: 2
                        shadowBlur: 4
                    }
                    
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 10
                        
                        Text {
                            text: "인증번호 5자리를 입력하세요"
                            font.pixelSize: 16
                            font.bold: true
                            color: hanwhaBlack
                        }
                        
                        TextField {
                            id: verificationField
                            Layout.fillWidth: true
                            placeholderText: "인증번호"
                            font.pixelSize: 16
                            inputMethodHints: Qt.ImhDigitsOnly
                            maximumLength: 5
                            validator: IntValidator {
                                bottom: 0
                                top: 99999
                            }
                            
                            background: Rectangle {
                                color: "white"
                                border.color: verificationField.focus ? hanwhaOrange100 : "#CCCCCC"
                                border.width: 1
                                radius: 4
                            }
                        }
                        
                        Text {
                            text: "* 테스트용으로 아무 5자리 숫자나 입력하세요"
                            font.pixelSize: 12
                            color: textSecondary
                        }
                    }
                }
                
                Item { Layout.fillHeight: true }
                
                // 다음 버튼
                Button {
                    Layout.fillWidth: true
                    text: "다음"
                    font.pixelSize: 16
                    enabled: verificationField.text.length === 5
                    
                    background: Rectangle {
                        color: parent.enabled ? 
                               (parent.down ? hanwhaOrange50 : (parent.hovered ? hanwhaOrange70 : hanwhaOrange100)) : 
                               "#CCCCCC"
                        radius: 4
                        
                        // 그림자 효과
                        layer.enabled: true
                        layer.effect: MultiEffect {
                            shadowEnabled: true
                            shadowColor: Qt.rgba(0, 0, 0, 0.2)
                            shadowHorizontalOffset: 0
                            shadowVerticalOffset: 2
                            shadowBlur: 4
                        }
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    
                    onClicked: {
                        // 인증번호는 아무 5자리 숫자나 통과
                        currentScreen = "complete"
                        
                        // 거래 처리
                        if (transactionType === "deposit") {
                            bankModel.deposit(selectedAccount, transactionAmount, verificationField.text)
                        } else if (transactionType === "withdraw") {
                            bankModel.withdraw(selectedAccount, transactionAmount, verificationField.text)
                        } else if (transactionType === "transfer") {
                            bankModel.transfer(selectedAccount, targetAccount, transactionAmount, verificationField.text)
                        }
                        
                        stackView.push(completeScreen)
                    }
                }
            }
        }
    }
    
    // 완료 화면
    Component {
        id: completeScreen
        
        Rectangle {
            color: backgroundColor
            
            property string completeText: transactionType === "deposit" ? "입금이 완료되었습니다" : 
                                         (transactionType === "withdraw" ? "출금이 완료되었습니다" : "송금이 완료되었습니다")
            property color iconColor: transactionType === "deposit" ? successColor : 
                                     (transactionType === "withdraw" ? warningColor : linkColor)
            
            ColumnLayout {
                anchors.centerIn: parent
                width: 300
                spacing: 30
                
                Rectangle {
                    Layout.alignment: Qt.AlignHCenter
                    width: 120
                    height: 120
                    radius: 60
                    color: "white"
                    
                    // 그림자 효과
                    layer.enabled: true
                    layer.effect: MultiEffect {
                        shadowEnabled: true
                        shadowColor: Qt.rgba(0, 0, 0, 0.1)
                        shadowHorizontalOffset: 0
                        shadowVerticalOffset: 2
                        shadowBlur: 4
                    }
                    
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/images/complete_icon.png"
                        width: 80
                        height: 80
                        fillMode: Image.PreserveAspectFit
                    }
                }
                
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: completeText
                    font.pixelSize: 24
                    font.bold: true
                    color: hanwhaBlack
                }
                
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: transactionAmount.toLocaleString() + " 원"
                    font.pixelSize: 20
                    color: hanwhaOrange100
                }
                
                Button {
                    Layout.fillWidth: true
                    text: "계좌 관리로 돌아가기"
                    font.pixelSize: 16
                    
                    background: Rectangle {
                        color: parent.down ? hanwhaOrange50 : (parent.hovered ? hanwhaOrange70 : hanwhaOrange100)
                        radius: 4
                        
                        // 그림자 효과
                        layer.enabled: true
                        layer.effect: MultiEffect {
                            shadowEnabled: true
                            shadowColor: Qt.rgba(0, 0, 0, 0.2)
                            shadowHorizontalOffset: 0
                            shadowVerticalOffset: 2
                            shadowBlur: 4
                        }
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    
                    onClicked: {
                        currentScreen = "account"
                        stackView.pop(accountScreen)
                    }
                }
            }
        }
    }
    
    // 로그인 오류 다이얼로그
    Dialog {
        id: loginErrorDialog
        title: "로그인 오류"
        standardButtons: Dialog.Ok
        
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        
        Text {
            text: "아이디 또는 비밀번호가 올바르지 않습니다."
        }
    }
}
