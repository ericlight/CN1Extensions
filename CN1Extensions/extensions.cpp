#include "extensions.h"
#include "ui_extensions.h"

#include <QTextStream>
#include <QFile>
#include <QDesktopServices>
#include <QDomDocument>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QClipboard>
#include <QProgressDialog>

Extensions::Extensions(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Extensions)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/new/prefix1/images/cn1.png"));

    ui->libInfosTable->setRowCount(8);

    ui->libInfosTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->libInfosTable->verticalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->libInfosTable->verticalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->libInfosTable->verticalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->libInfosTable->verticalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->libInfosTable->verticalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->libInfosTable->verticalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->libInfosTable->verticalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->libInfosTable->verticalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);

    ui->refreshExt->setFocus();

    tabifyDockWidget(ui->extensionsDockWidget, ui->dwnldExtDockWidget);
    ui->extensionsDockWidget->raise();

    QAction *actualiserListe=new QAction(QIcon(":/new/prefix1/images/refresh1.png"), tr("Refresh list"), this);
    QAction *ajouterAUnProjet=new QAction(QIcon(":/new/prefix1/images/plus.png"), tr("Add to a project..."), this);
    QAction *supprimerLib=new QAction(QIcon(":/new/prefix1/images/supprimer.png"), tr("Remove library..."), this);
    QAction *supprimerLibs=new QAction(QIcon(":/new/prefix1/images/supprimer.png"), tr("Remove all libraries..."), this);
    QAction *voirLibs=new QAction(tr("Go to Repository Libraries"), this);

    connect(actualiserListe, &QAction::triggered, this, [=](){
        on_refreshExtDwlded_clicked();
    });

    connect(ajouterAUnProjet, &QAction::triggered, this, [=](){
        on_addLibToProject_clicked();

    });

    connect(supprimerLib, &QAction::triggered, this, [=](){
        supprimerLibTelecharge();
    });

    connect(supprimerLibs, &QAction::triggered, this, [=](){
        supprimerLibsTelecharges();
    });

    connect(voirLibs, &QAction::triggered, this, [=](){
        ui->extensionsDockWidget->raise();
    });

    ui->downloadExtList->addAction(actualiserListe);
    ui->downloadExtList->addAction(ajouterAUnProjet);
    ui->downloadExtList->addAction(supprimerLib);
    ui->downloadExtList->addAction(supprimerLibs);
    QAction *separateur=new QAction(this);
    separateur->setSeparator(true);
    ui->downloadExtList->addAction(separateur);
    ui->downloadExtList->addAction(voirLibs);
    ui->downloadExtList->setContextMenuPolicy(Qt::ActionsContextMenu);

    QAction *telechargerLib=new QAction(QIcon(":/new/prefix1/images/cn1.png"), tr("Download library"), this);
    QAction *copierLibURL=new QAction(QIcon(":/new/prefix1/images/copy.png"), tr("Copy URL"), this);
    QAction *ouvrirLibHomepage=new QAction(QIcon(":/new/prefix1/images/web.png"), tr("Library homepage"), this);
    QAction *voirLibsTelecharges=new QAction(tr("Go to Downloaded Libraries"), this);

    ui->extList->addAction(telechargerLib);
    ui->extList->addAction(copierLibURL);
    ui->extList->addAction(ouvrirLibHomepage);
    QAction *separateur2=new QAction(this);
    separateur2->setSeparator(true);
    ui->extList->addAction(separateur2);
    ui->extList->addAction(voirLibsTelecharges);
    ui->extList->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(telechargerLib, &QAction::triggered, this, [=](){
        on_downloadExt_clicked();
    });
    connect(copierLibURL, &QAction::triggered, this, [=](){
        on_copyURL_clicked();
    });
    connect(ouvrirLibHomepage, &QAction::triggered, this, [=](){
        on_visitWebsite_clicked();
    });
    connect(voirLibsTelecharges, &QAction::triggered, this, [=](){
        ui->dwnldExtDockWidget->raise();
    });
    //---------------

    QAction *desinstallerLib=new QAction(QIcon(":/new/prefix1/images/supprimer.png"), tr("Uninstall library"), this);
    ui->projectsLibsList->addAction(desinstallerLib);
    ui->projectsLibsList->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(desinstallerLib, &QAction::triggered, this, [=](){
        on_uninstallLib_clicked();
    });

    QAction *afficherMasquerZoneProjets=ui->projectsDockWidget->toggleViewAction();
    afficherMasquerZoneProjets->setEnabled(true);
    afficherMasquerZoneProjets->setText("Projects");
    connect(afficherMasquerZoneProjets, &QAction::triggered, this, [=](bool checked){
        QSettings settings("cn1ext-config.ini", QSettings::IniFormat);
        settings.beginGroup("General");
        settings.setValue("project_panel_active", checked);
        settings.endGroup();
    });

    QSettings settings("cn1ext-config.ini", QSettings::IniFormat);
    settings.beginGroup("General");
    ui->projectsFolderLocation->setText(settings.value("project_folder_path").toString());
    if(settings.value("project_panel_active").toBool()==true){
        afficherMasquerZoneProjets->setChecked(true);
        ui->projectsDockWidget->setVisible(true);

    } else {
        ui->projectsDockWidget->setVisible(false);
    }
    ui->actionEnable_Disable_Fusion_Theme->setChecked(settings.value("ThemeFusion").toBool());
    settings.endGroup();

    ui->menuView->addAction(afficherMasquerZoneProjets);

    on_refreshExtDwlded_clicked();

    netManager=new QNetworkAccessManager(this);
}

Extensions::~Extensions()
{
    delete ui;
}

void Extensions::on_visitWebsite_clicked()
{
    if(ui->extList->currentItem()==nullptr){
        return;
    }

    QString url=ui->libInfosTable->item(2,0)->text();
    QDesktopServices::openUrl(url.mid(6,url.length()));
}

void Extensions::on_downloadExt_clicked()
{
    if(ui->extList->currentItem()==nullptr){
        return;
    }

    ui->downloadExt->setEnabled(false);

    QNetworkRequest request;
//    request.setUrl(QUrl("http://localhost:8080/"+ui->extList->currentItem()->toolTip()));
    request.setUrl(QUrl(DOSSIER_GITHUB_CN1LIBS+ui->extList->currentItem()->toolTip()+"?raw=true"));

    QNetworkReply *response=netManager->get(request);

    QProgressDialog *progress=new QProgressDialog("Downloading <b>"+ui->extList->currentItem()->toolTip()+"</b>...", "Cancel download", 0, 100, this);
    progress->setWindowModality(Qt::WindowModal);
    progress->show();

    connect(progress, &QProgressDialog::canceled, this, [=](){
        response->abort();
        progress->cancel();
        progress->close();

        QFile cn1libFile("libs/"+ui->extList->currentItem()->toolTip());
        if(cn1libFile.exists()){
            cn1libFile.remove();
        }
    });

    connect(response, &QNetworkReply::downloadProgress, this, [=](qint64 received, qint64 total){
        if(received>0 && total>0){
            pourcentage=(received*100)/total;
            progress->setValue(int(pourcentage));
        } else {
            progress->cancel();
        }
    });

    connect(response,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(erreurTelCN1Lib(QNetworkReply::NetworkError)));
    connect(response,&QNetworkReply::finished,this,[=](){

        if(response->bytesAvailable()){
            QFile cn1libFile("libs/"+ui->extList->currentItem()->toolTip());
            cn1libFile.open(QIODevice::WriteOnly);
            cn1libFile.write(response->readAll());
            cn1libFile.close();

            if(cn1libFile.flush()){
                QMessageBox::information(this,tr("Succeed"),tr("Download completed successfully"));
            }
            else {
                QMessageBox::critical(this,tr("Failed"),tr("Download failed! Please retry later"));
                cn1libFile.remove();
            }
        }

        ui->downloadExt->setEnabled(true);

        response->close();
        response->deleteLater();
    });
}

void Extensions::erreurTelCN1Lib(QNetworkReply::NetworkError error)
{
    if(error==QNetworkReply::ConnectionRefusedError){
        QMessageBox::critical(this, tr("Error"), tr("An error has occured. Check your internet connexion."));
    } else {
        QMessageBox::critical(this,tr("Failed"),tr("Download failed! Please retry later"));
    }

    ui->downloadExt->setEnabled(true);

    QFile cn1libFile("libs/"+ui->extList->currentItem()->toolTip());
    if(cn1libFile.exists()){
        cn1libFile.remove();
    }
}

void Extensions::erreurTelListeLibs(QNetworkReply::NetworkError error)
{
    if(error==QNetworkReply::ConnectionRefusedError){
        QMessageBox::critical(this, tr("Error"), tr("Error! Check if your internet connexion is active."));
    } else {
        QMessageBox::critical(this,tr("Failed"),tr("Libraries list download failed! Please retry later"));
    }

    ui->refreshExt->setEnabled(true);

    QFile f("CN1Libs.xml");
    if(f.exists()){
        f.remove();
    }
}

void Extensions::on_refreshExt_clicked()
{
    ui->refreshExt->setEnabled(false);

    QNetworkRequest request;
//    request.setUrl(QUrl("http://localhost:8080/CN1Libs.xml"));
    request.setUrl(QUrl("https://raw.githubusercontent.com/codenameone/CodenameOneLibs/master/CN1Libs.xml"));

    QNetworkReply *response=netManager->get(request);

    QProgressDialog *progress=new QProgressDialog("Loading libraries...", nullptr, 0, 100, this);
    progress->setWindowModality(Qt::WindowModal);
    progress->show();

    connect(response, &QNetworkReply::downloadProgress, this, [=](qint64 received, qint64 total){
        if(received>0 && total>0){
            pourcentage=(received*100)/total;
            progress->setValue(int(pourcentage));
        } else {
            progress->cancel();
        }
    });

    connect(response,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(erreurTelListeLibs(QNetworkReply::NetworkError)));
    connect(response,&QNetworkReply::finished,this,[=](){

        if(response->bytesAvailable()){
            QFile libsList("CN1Libs.xml");
            libsList.open(QIODevice::WriteOnly);
            QTextStream stream(&libsList);
            stream<<response->readAll();
            libsList.close();

            if(libsList.exists()){
                QDomDocument dom;

                if(!libsList.open(QIODevice::ReadOnly)) {
                    //QMessageBox::warning(this, "Erreur à l'ouverture du document", "Le document <b>grille.xml</b> n'a pas pu être ouvert.");
                    return;
                }

                if(!dom.setContent(&libsList)) {
                     libsList.close();
                     //QMessageBox::warning(this, "Erreur à l'ouverture du document XML", "Le document <b>grille.xml</b> n'a pas pu être attribué à l'objet QDomDocument.");
                     return;
                }
                libsList.close();

                QDomElement pluginsElement=dom.documentElement();
                QDomNodeList pluginNodesList=pluginsElement.childNodes();
                int pluginNbreNode=pluginNodesList.count();

                libsData.clear();
                ui->extList->clear();

                for(int i=0;i<pluginNbreNode;++i) {

                    QDomElement pluginElement=pluginNodesList.at(i).toElement();
                    QDomNodeList pluginChildNodesList=pluginElement.childNodes();

                    QDomElement elementName=pluginChildNodesList.at(0).toElement();
                    QDomElement elementDescr=pluginChildNodesList.at(1).toElement();
                    QDomElement elementURL=pluginChildNodesList.at(2).toElement();
                    QDomElement elementVersion=pluginChildNodesList.at(3).toElement();
                    QDomElement elementLicense=pluginChildNodesList.at(4).toElement();
                    QDomElement elementTags=pluginChildNodesList.at(5).toElement();
                    QDomElement elementPlatforms=pluginChildNodesList.at(6).toElement();
                    QDomElement elementDep=pluginChildNodesList.at(7).toElement();
                    QDomElement elementAuthor=pluginChildNodesList.at(8).toElement();
                    QString pluginFileName=pluginElement.attribute("fileName");

                    QMap<QString,QString> libData;
                    libData.insert("fileName",pluginFileName);
                    libData.insert("name",elementName.text());
                    libData.insert("description",elementDescr.text());
                    libData.insert("url",elementURL.text());
                    libData.insert("version",elementVersion.text());
                    libData.insert("license",elementLicense.text());
                    libData.insert("tags",elementTags.text());
                    libData.insert("platforms",elementPlatforms.text());
                    libData.insert("dep",elementDep.text());
                    libData.insert("author",elementAuthor.text());
                    libsData.push_back(libData);

                    QListWidgetItem *libItem=new QListWidgetItem(elementName.text());
                    libItem->setIcon(QIcon(":/new/prefix1/images/item.png"));
                    libItem->setToolTip(pluginFileName);
                    ui->extList->addItem(libItem);

                    QDir dossierLibs("libs");
                    QStringList fichiersCN1LIBTel=dossierLibs.entryList(QDir::Files);
                    if(fichiersCN1LIBTel.contains(pluginFileName)){
                        libItem->setBackground(QColor("#60be92"));
                    }
                }

                int nbreLib=ui->extList->count();
                if(nbreLib>1){
                    ui->nbreExt->setText(QString::number(nbreLib)+" libraries");
                } else {
                    ui->nbreExt->setText(QString::number(nbreLib)+" library");
                }
            }
        }

        ui->refreshExt->setEnabled(true);
        response->close();
        response->deleteLater();
    });
}

void Extensions::on_action_Quit_triggered()
{
    qApp->quit();
}

void Extensions::on_extNameSearch_textEdited(const QString &arg1)
{
    ui->extList->clear();
    for (QMap<QString,QString> libData : libsData) {
        if(libData.value("name").toLower().contains(arg1) ||
           libData.value("tags").toLower().contains(arg1) ||
           libData.value("author").toLower().contains(arg1))
        {
            QListWidgetItem *libItem=new QListWidgetItem(libData.value("name"));
            libItem->setIcon(QIcon(":/new/prefix1/images/item.png"));
            libItem->setToolTip(libData.value("fileName"));
            ui->extList->addItem(libItem);

            QDir dossierLibs("libs");
            QStringList fichiersCN1LIBTel=dossierLibs.entryList(QDir::Files);
            if(fichiersCN1LIBTel.contains(libData.value("fileName"))){
                libItem->setBackground(QColor("#60be92"));
            }
        }
    }

    int nbreLib=ui->extList->count();
    if(nbreLib>1){
        ui->nbreExt->setText(QString::number(nbreLib)+" libraries");
    } else {
        ui->nbreExt->setText(QString::number(nbreLib)+" library");
    }
}

void Extensions::on_extList_itemSelectionChanged()
{
    for (QMap<QString,QString> libData : libsData) {
        QString name=libData.value("name");
        if(name==ui->extList->currentItem()->text()){

            QString dep=libData.value("dep");
            if(dep.isEmpty()){
                dep="N/A";
            }

            QTableWidgetItem *itemName=new QTableWidgetItem(name);
            itemName->setToolTip(name);
            QTableWidgetItem *itemAuthor=new QTableWidgetItem("Author : "+libData.value("author"));
            QTableWidgetItem *itemURL=new QTableWidgetItem("URL : "+libData.value("url"));
            QTableWidgetItem *itemVersion=new QTableWidgetItem("Version : "+libData.value("version"));
            QTableWidgetItem *itemLicense=new QTableWidgetItem("License : "+libData.value("license"));
            QTableWidgetItem *itemPlatforms=new QTableWidgetItem("Platforms : "+libData.value("platforms"));
            QTableWidgetItem *itemDep=new QTableWidgetItem("Dependencies : "+dep);
            if(dep!="N/A"){
                itemDep->setBackground(QColor("#e68888"));
                itemDep->setToolTip("You have to download these dependencies too");
            }
            QTableWidgetItem *itemTags=new QTableWidgetItem("Tags : "+libData.value("tags"));

            ui->libInfosTable->setItem(0,0,itemName);
            ui->libInfosTable->setItem(1,0,itemAuthor);
            ui->libInfosTable->setItem(2,0,itemURL);
            ui->libInfosTable->setItem(3,0,itemVersion);
            ui->libInfosTable->setItem(4,0,itemLicense);
            ui->libInfosTable->setItem(5,0,itemPlatforms);
            ui->libInfosTable->setItem(6,0,itemDep);
            ui->libInfosTable->setItem(7,0,itemTags);

            QFont f=ui->libInfosTable->font();
            ui->libInfosTable->item(0,0)->setFont(QFont(f.family(),14,QFont::Bold));

            ui->featuresAndDescriptions->setHtml(libData.value("description"));

            break;
        }
    }
}

void Extensions::on_chooseProjectsFolder_clicked()
{
    QString cheminDossierProjets=QFileDialog::getExistingDirectory(this,tr("Choose projects folder"),".");
    if(cheminDossierProjets.isEmpty()){
        return;
    }

    ui->projectsFolderLocation->setText(cheminDossierProjets);

    QSettings settings("cn1ext-config.ini", QSettings::IniFormat);
    settings.beginGroup("General");
    settings.setValue("project_folder_path", cheminDossierProjets);
    settings.endGroup();

    on_refreshFoldersList_clicked();
}

void Extensions::on_refreshFoldersList_clicked()
{
    ui->refreshFoldersList->setEnabled(false);

    QString cheminDossierProjets=ui->projectsFolderLocation->text();
    QDir dossierProjets(cheminDossierProjets);
    QStringList listeDossiers=dossierProjets.entryList(QDir::Dirs|QDir::NoDotAndDotDot);

    QProgressDialog *progress=new QProgressDialog("Loading projects...", nullptr, 0, 100, this);
    progress->setWindowModality(Qt::WindowModal);
    progress->show();

    ui->foldersList->clear();
    int position=1;
    int nbreDossiers=listeDossiers.count();

    for (QString dossier : listeDossiers) {

        QStringList filtre={"*.cn1lib"};
        QString cheminDossierLib=cheminDossierProjets+"/"+dossier+"/lib";
        QDir dossierLib(cheminDossierLib);
        QStringList fichiersCN1LIB=dossierLib.entryList(filtre, QDir::Files);

        if(fichiersCN1LIB.count()>0){
            QListWidgetItem *item=new QListWidgetItem(dossier);
            item->setIcon(QIcon(":/new/prefix1/images/Folder.png"));
            item->setToolTip(cheminDossierLib);
            ui->foldersList->addItem(item);         
        }

        pourcentage=(position*100)/nbreDossiers;
        progress->setValue(int(pourcentage));

        position++;
    }

    if(ui->projectsLibsList->count()>0){
        ui->projectsLibsList->clear();
    }

    ui->refreshFoldersList->setEnabled(true);
}

void Extensions::on_foldersList_itemSelectionChanged()
{
    QStringList filtre={"*.cn1lib"};
    QString cheminDossierLib=ui->foldersList->currentItem()->toolTip();
    QDir dossierLib(cheminDossierLib);
    QStringList fichiersCN1LIB=dossierLib.entryList(filtre, QDir::Files);

    ui->projectsLibsList->clear();
    for(QString fichierCN1LIB:fichiersCN1LIB){
        QListWidgetItem *item=new QListWidgetItem(fichierCN1LIB);
        item->setIcon(QIcon(":/new/prefix1/images/cn1.png"));
        item->setToolTip(cheminDossierLib+"/"+fichierCN1LIB);
        ui->projectsLibsList->addItem(item);
    }
}

void Extensions::on_uninstallLib_clicked()
{
    if(ui->projectsLibsList->count()==0){
        QMessageBox::warning(this,tr("Remove Libraries"),tr("There is no libraries to remove."));
        return;
    }

    if(ui->projectsLibsList->currentItem()==nullptr){
        QMessageBox::warning(this,tr("Remove Libraries"),tr("You have to select the library that you want to remove."));
        return;
    }

    int reponse=QMessageBox::question(this,tr("Confirm"),tr("Do you really want to remove this library from this project ?"), QMessageBox::Yes|QMessageBox::No);
    if(reponse==QMessageBox::Yes){
        QFile fichierLib(ui->projectsLibsList->currentItem()->toolTip());
        QDir dir(ui->foldersList->currentItem()->toolTip());
        dir.cdUp();
        QString projectFolderPath=dir.path();

        if(fichierLib.exists()){

            bool confirmation=fichierLib.remove();
            if(confirmation){
                QStringList args;
                args<<"-refresh";
                args<<projectFolderPath;
                bool ok=QProcess::startDetached("refresh-libs",args);

                QListWidgetItem *item_retire=ui->projectsLibsList->takeItem(ui->projectsLibsList->currentRow());
                delete item_retire;

                if(ok==true){
                    QMessageBox::information(this,tr("Remove Libraries"),tr("Library removed and project refreshed successfully !"));

                } else {
                    QMessageBox::warning(this,tr("Remove Libraries"),tr("Library removed but project not refreshed! Go back to your project and <b>make a refresh libs</b> manually to complete the process."));
                }

            } else {
                QMessageBox::critical(this,tr("Remove Libraries"),tr("An error has occured. The library is not removed so try again"));
            }
        }
    }
}

void Extensions::on_foldersList_itemDoubleClicked(QListWidgetItem *item)
{
    QDir dossierLib(item->toolTip());
    dossierLib.cdUp();

    QDesktopServices::openUrl(dossierLib.path());
}

void Extensions::on_projectsLibsList_itemDoubleClicked(QListWidgetItem *item)
{
    QFileInfo fichierLib(item->toolTip());
    QDesktopServices::openUrl(fichierLib.absolutePath());
}

void Extensions::on_copyURL_clicked()
{
    if(ui->extList->currentItem()==nullptr){
        return;
    }

    QClipboard *clipboard=qApp->clipboard();
    QString url=ui->libInfosTable->item(2,0)->text();
    clipboard->setText(url.mid(6,url.length()));
    statusBar()->showMessage("URL copied",2000);
}

void Extensions::on_refreshExtDwlded_clicked()
{
    QDir dossierLibs("libs");
    QStringList filtre={"*.cn1lib"};
    QStringList fichiersLibs=dossierLibs.entryList(filtre, QDir::Files);

    ui->downloadExtList->clear();
    for (QString lib : fichiersLibs) {
        QListWidgetItem *item=new QListWidgetItem(lib);
        item->setIcon(QIcon(":/new/prefix1/images/cn1.png"));
        item->setToolTip(dossierLibs.absolutePath()+"/"+lib);
        ui->downloadExtList->addItem(item);
    }

    int nbreLib=ui->downloadExtList->count();
    if(nbreLib>1){
        ui->nbreExtTel->setText(QString::number(nbreLib)+" libraries");
    } else {
        ui->nbreExtTel->setText(QString::number(nbreLib)+" library");
    }
}

void Extensions::supprimerLibTelecharge()
{
    if(ui->downloadExtList->selectedItems().size()==0) {
        QMessageBox::warning(this,tr("No library selected"),tr("You have to select the library you want to delete."));
        return;
    }

    QFile cn1libFile(ui->downloadExtList->currentItem()->toolTip());
    bool ok=cn1libFile.remove();
    if(ok){
        QListWidgetItem *item=ui->downloadExtList->takeItem(ui->downloadExtList->currentRow());
        delete item;
    }

    int nbreLib=ui->downloadExtList->count();
    if(nbreLib>1){
        ui->nbreExtTel->setText(QString::number(nbreLib)+" libraries");
    } else {
        ui->nbreExtTel->setText(QString::number(nbreLib)+" library");
    }
}

void Extensions::supprimerLibsTelecharges()
{
    QStringList filtre={"*.cn1lib"};
    QDir dossierLibs("libs");
    QStringList fichiersCN1LIB=dossierLibs.entryList(filtre, QDir::Files);

    for(QString fichierCN1LIB:fichiersCN1LIB){
        QFile f(QDir::currentPath()+"/libs/"+fichierCN1LIB);
        f.remove();
    }

    ui->downloadExtList->clear();

    int nbreLib=ui->downloadExtList->count();
    if(nbreLib>1){
        ui->nbreExtTel->setText(QString::number(nbreLib)+" libraries");
    } else {
        ui->nbreExtTel->setText(QString::number(nbreLib)+" library");
    }
}

void Extensions::on_downloadExtList_itemDoubleClicked(QListWidgetItem *item)
{
    QFileInfo fichierLib(item->toolTip());
    QDesktopServices::openUrl(fichierLib.absolutePath());
}

void Extensions::on_addLibToProject_clicked()
{
    if(ui->downloadExtList->selectedItems().size()==0) {
        QMessageBox::warning(this,tr("No library selected"),tr("You have to select a downloaded library before."));
        return;
    }

    QString cheminDossierProjet=QFileDialog::getExistingDirectory(this,tr("Choose project folder"),".");
    if(cheminDossierProjet.isEmpty()){
        return;
    }

    QFile fichierCN1LIB(ui->downloadExtList->currentItem()->toolTip());
    fichierCN1LIB.open(QIODevice::ReadOnly);

    QFileInfo info(ui->downloadExtList->currentItem()->toolTip());
    QFile fichierCN1LIBNew(cheminDossierProjet+"/lib/"+info.fileName());
    fichierCN1LIBNew.open(QIODevice::WriteOnly);

    fichierCN1LIBNew.write(fichierCN1LIB.readAll());
    if(fichierCN1LIBNew.flush()){
        QStringList args;
        args<<"-refresh";
        args<<cheminDossierProjet;

        bool ok=QProcess::startDetached("refresh-libs", args);
        if(ok){
            QMessageBox::information(this,tr("Succeed"),tr("Library added and project refreshed successfully !"));
        } else {
            QMessageBox::warning(this,tr("Succeed partially"),tr("Library added but project not refreshed! Go back to your project and <b>make a refresh libs</b> manually to complete the process."));
        }

    } else {
        QMessageBox::critical(this,tr("Failed"),tr("Library not added. Please retry and be sure to choose a Codename One project"));
        fichierCN1LIBNew.remove();
    }

    fichierCN1LIB.close();
    fichierCN1LIBNew.close();
}

void Extensions::on_actionAbout_triggered()
{
    QMessageBox::about(this,tr("About Codename One Extensions"), tr("<b>Codename One Extensions</b> is a desktop tool to manage Codename One libraries (CN1LIB) in your projects. You can use it to download, install, uninstall a CN1LIB from a project.<br/><br/>"
                                                               "Built by <b>Eric Dodji Gbofu</b> for Codename One developers community<br/><br/>"
                                                               "<u>GitHub :</u> <a href='"+CN1EXTENSIONS_GITHUB_PAGE.toLocal8Bit()+"'><u>"+CN1EXTENSIONS_GITHUB_PAGE.toLocal8Bit()+"</u></a>"));
}

void Extensions::on_actionEnable_Disable_Fusion_Theme_triggered()
{
    QSettings settings("cn1ext-config.ini", QSettings::IniFormat);
    settings.beginGroup("General");
    settings.setValue("ThemeFusion", ui->actionEnable_Disable_Fusion_Theme->isChecked());
    settings.endGroup();

    int reponse=QMessageBox::information(this,tr("Restart needed"),tr("You have to restart the application to apply the new theme. Do you want to do it now ?"), QMessageBox::Yes|QMessageBox::No);
    if(reponse==QMessageBox::Yes){
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0],QStringList());
    }
}
