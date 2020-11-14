#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <QDomDocument>
#include <QListWidget>
#include <QMainWindow>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class Extensions; }
QT_END_NAMESPACE

class Extensions : public QMainWindow
{
    Q_OBJECT

public:
    Extensions(QWidget *parent = nullptr);
    ~Extensions();

private slots:
    void on_visitWebsite_clicked();

    void on_downloadExt_clicked();

    void on_refreshExt_clicked();

    void on_action_Quit_triggered();

    void on_extNameSearch_textEdited(const QString &arg1);

    void on_extList_itemSelectionChanged();

    void on_chooseProjectsFolder_clicked();

    void on_refreshFoldersList_clicked();

    void on_foldersList_itemSelectionChanged();

    void on_uninstallLib_clicked();

    void on_foldersList_itemDoubleClicked(QListWidgetItem *item);

    void on_projectsLibsList_itemDoubleClicked(QListWidgetItem *item);

    void on_copyURL_clicked();

    void erreurTelCN1Lib(QNetworkReply::NetworkError);

    void on_refreshExtDwlded_clicked();

    void on_addLibToProject_clicked();

    void on_actionAbout_triggered();

    void on_downloadExtList_itemDoubleClicked(QListWidgetItem *item);

    void on_actionEnable_Disable_Fusion_Theme_triggered();

    void erreurTelListeLibs(QNetworkReply::NetworkError);

private:
    Ui::Extensions *ui;
    QList<QMap<QString,QString>> libsData;
    QNetworkAccessManager *netManager;
    qint64 pourcentage=0;

    const QString CN1EXTENSIONS_GITHUB_PAGE="https://github.com/ericlight/CN1Extensions";
    const QString DOSSIER_GITHUB_CN1LIBS="https://github.com/codenameone/CodenameOneLibs/blob/master/cn1libs/";

    void supprimerLibTelecharge();
    void supprimerLibsTelecharges();

};
#endif // EXTENSIONS_H
