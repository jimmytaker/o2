#ifndef O1_H
#define O1_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>

class O2ReplyServer;
class SimpleCrypt;

/// Simple OAuth 1.0 authenticator.
class O1: public QObject {
    /// Are we authenticated?
    Q_PROPERTY(bool linked READ linked NOTIFY linkedChanged)
    bool linked();

    /// Authentication token.
    QString token();

    /// Authentication token secret..
    QString tokenSecret();

    /// Client ID.
    /// O1 instances with the same (client ID, client secret) share the same "linked" and "token" properties.
    Q_PROPERTY(QString clientId READ clientId WRITE setClientId NOTIFY clientIdChanged)
    QString clientId();
    void setClientId(const QString &value);

    /// Client secret.
    /// O1 instances with the same (client ID, client secret) share the same "linked" and "token" properties.
    Q_PROPERTY(QString clientSecret READ clientSecret WRITE setClientSecret NOTIFY clientSecretChanged)
    QString clientSecret();
    void setClientSecret(const QString &value);

    /// Token request URL.
    Q_PROPERTY(QUrl requestTokenUrl READ requestTokenUrl WRITE setRequestTokenUrl NOTIFY requestTokenUrlChanged)
    QUrl requestTokenUrl();
    void setRequestTokenUrl(const QUrl &value);

    /// Authorization URL.
    Q_PROPERTY(QUrl authorizeUrl READ authorizeUrl WRITE setAuthorizeUrl NOTIFY authorizeUrlChanged)
    QUrl authorizeUrl();
    void setAuthorizeUrl(const QUrl &value);

    /// Access token URL.
    Q_PROPERTY(QUrl accessTokenUrl READ accessTokenUrl WRITE setAccessTokenUrl NOTIFY accessTokenUrlChanged)
    QUrl accessTokenUrl();
    void setAccessTokenUrl(const QUrl &value);

    /// TCP port number to use in local redirections.
    /// The OAuth "redirect_uri" will be set to "http://localhost:<localPort>/".
    /// If localPort is set to 0 (default), O1 will replace it with a free one.
    Q_PROPERTY(int localPort READ localPort WRITE setLocalPort NOTIFY localPortChanged)
    int localPort();
    void setLocalPort(int value);

public:
    /// Constructor.
    /// @param  parent  Parent object.
    explicit O1(QObject *parent = 0);

    /// Destructor.
    virtual ~O1();

    /// Get authentication code.
    QString code();

    /// Get refresh token.
    QString refreshToken();

public slots:
    /// Authenticate.
    Q_INVOKABLE virtual void link();

    /// De-authenticate.
    Q_INVOKABLE virtual void unlink();

signals:
    /// Emitted when client needs to open a web browser window, with the given URL.
    void openBrowser(const QUrl &url);

    /// Emitted when client can close the browser window.
    void closeBrowser();

    /// Emitted when authentication/deauthentication succeeded.
    void linkingSucceeded();

    /// Emitted when authentication/deauthentication failed.
    void linkingFailed();

    // Property change signals
    void linkedChanged();
    void clientIdChanged();
    void clientSecretChanged();
    void requestTokenUrlChanged();
    void authorizeUrlChanged();
    void accessTokenUrlChanged();
    void localPortChanged();

protected slots:
    /// Handle verification received from the reply server.
    void onVerificationReceived(QMap<QString,QString> params);

    /// Handle token request error.
    void onTokenRequestError(QNetworkReply::NetworkError error);

    /// Handle token request finished.
    void onTokenRequestFinished();

protected:
    /// Set authentication code.
    void setCode(const QString &v);

    /// Set authentication token.
    void setToken(const QString &v);

    /// Set authentication token secret.
    void setTokenSecret(const QString &v);

    /// Set refresh token.
    void setRefreshToken(const QString &v);

protected:
    QString clientId_;
    QString clientSecret_;
    QString scope_;
    QUrl requestTokenUrl_;
    QUrl authorizeUrl_;
    QUrl accessTokenUrl_;
    QString redirectUri_;
    QNetworkAccessManager *manager_;
    O2ReplyServer *replyServer_;
    QString code_;
    SimpleCrypt *crypt_;
    quint16 localPort_;
};

#endif // O1_H