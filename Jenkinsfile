pipeline {
    agent any
    stages {
        stage('Checkout VVS Incidents') {
            steps {
                git branch: 'main',
                        url: 'https://github.com/andywuest/harbour-vvs-incidents.git'
            }
        }
        stage('Run C++ tests') {
            steps {
                sh "cd tests && ls -l && bash runTests.sh"
            }
        }
        stage('Run QML test') {
            steps {
                sh "cd tests_qml && ls -l && bash runTests.sh"
            }
        }
    }
    post {
        always {
            xunit(
                    thresholds: [skipped(failureThreshold: '0'), failed(failureThreshold: '0')],
                    tools: [QtTest(pattern: '**/*results.xml'), ]
            )
        }
    }
}
