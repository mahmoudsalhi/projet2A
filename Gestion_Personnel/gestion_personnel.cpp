#include "gestion_personnel.h"
#include "forgetp.h"
#include "ui_gestion_personnel.h"
//#include <Python.h>
//BEGIN CONSTRUCTOR
Gestion_Personnel::Gestion_Personnel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Gestion_Personnel)
{
    ui->setupUi(this);

    //BEGIN ONLY DIGITS
    QRegExpValidator *validatorNum = new QRegExpValidator(QRegExp("[0-9]*"));
    ui->CIN->setValidator(validatorNum);
    ui->salaire->setValidator(validatorNum);
    ui->numtel->setValidator(validatorNum);
    //END ONLY DIGITS

    //BEGIN ONLY LETTERS
    QRegExpValidator *validatorLet = new QRegExpValidator(QRegExp("[A-Za-z]*"));
    ui->nom->setValidator(validatorLet);
    ui->prenom->setValidator(validatorLet);
    //END ONLY LETTERS

    //BEGIN ONLY LETTERS AND DIGITS
    QRegExpValidator *validatorLetNum = new QRegExpValidator(QRegExp("[A-Za-z0-9]*"));
    ui->adresse->setValidator(validatorLetNum);
    //END ONLY LETTERS AND DIGITS

    //BEGIN affichage par defaut
    detectAfficher(0);
    //END affichage par defaut

    //BEGIN filterTable
    connect(ui->search, &QLineEdit::textChanged, this, &Gestion_Personnel::filterTable);
    //END filterTable

    //BEGIN trier par salaire
    connect(ui->triS, &QPushButton::clicked, this, [this]() {
        if(ui->asc->isChecked()){
            detectAfficher(1);
        }
        else if(ui->desc->isChecked()){
            detectAfficher(2);
        }

    });
    //END trier par salaire

    //BEGIN trier par date de naissance
    connect(ui->triD, &QPushButton::clicked, this, [this]() {
        if(ui->asc->isChecked()){
            detectAfficher(3);
        }
        else if(ui->desc->isChecked()){
            detectAfficher(4);
        }

    });
    //END trier par date de naissance

    //BEGIN trier par nom asc
    connect(ui->ascN, &QPushButton::clicked, this, [this]() {
        detectAfficher(5);
    });
    //END trier par nom asc

    //BEGIN trier par nom desc
    connect(ui->descN, &QPushButton::clicked, this, [this]() {
        detectAfficher(6);
    });
    //END trier par nom desc

    //BEGIN menu
    connect(ui->menu, &QPushButton::clicked, this, [this]() {
        Menu* w = new Menu();
        w->show();
        this->close();
    });
    //END menu

    //BEGIN ajouter
    connect(ui->ajouter, &QPushButton::clicked, this, &Gestion_Personnel::ajouterPersonnel);
    //END ajouter

    //BEGIN statistc
    connect(ui->stat, &QPushButton::clicked, this, &Gestion_Personnel::statistic);
    //END statistc

    //BEGIN statOFF
    connect(ui->statOFF, &QPushButton::clicked, this, &Gestion_Personnel::statisticOFF);
    //END statOFF

    //BEGIN assistance
    connect(ui->confirm, &QPushButton::clicked, this, &Gestion_Personnel::assistance);
    //END assistance

    //BEGIN DownloadPDF
    connect(ui->downloadPDF, &QPushButton::clicked, this, &Gestion_Personnel::downloadPDF);
    //END DownloadPDF

    //BEGIN scan_CIN
    connect(ui->scan_CIN, &QPushButton::clicked, this, &Gestion_Personnel::scan_CIN);
    //END scan_CIN

    //BEGIN scan_Face
    connect(ui->scan_Face, &QPushButton::clicked, this, &Gestion_Personnel::scan_Face);
    //END scan_Face

    //BEGIN valideCIN
    connect(ui->CIN, &QLineEdit::textChanged, this, [this]() {
        if(!valideCIN(ui->CIN->text())){
            showError(ui->CIN);
        }
        else {
            hideError(ui->CIN);
        }
    });
    //END valideCIN

    //BEGIN valideNom
    connect(ui->nom, &QLineEdit::textChanged, this, [this]() {
        if(ui->nom->text().isEmpty() || ui->nom->text().length()<3 ){
            showError(ui->nom);
        }
        else {
            hideError(ui->nom);
        }
    });
    //END valideNom

    //BEGIN validePreNom
    connect(ui->prenom, &QLineEdit::textChanged, this, [this]() {
        if(ui->prenom->text().isEmpty() || ui->prenom->text().length()<3 ){
            showError(ui->prenom);
        }
        else {
            hideError(ui->prenom);
        }
    });
    //END validePreNom

    //BEGIN valideDateNaiss
    connect(ui->age, &QDateEdit::dateChanged, this, [this]() {
        if(!valideDateNaiss(ui->age->date())){
            ui->age->setStyleSheet(
                "QDateEdit {"
                "    padding: 5px;"
                "    border: 2px solid red;"
                "    border-radius: 5px;"
                "    font-size: 14px;"
                "    color: #333;"
                "}"
                "QDateEdit::drop-down {"
                "    subcontrol-origin: padding;"
                "    subcontrol-position: top right;"
                "    width: 20px;"
                "    border-left: 2px solid #3498db;"
                "}"
            );
        }
        else {
            ui->age->setStyleSheet(
                "QDateEdit {"
                "    padding: 5px;"
                "    border: 2px solid #43aff7;"
                "    border-radius: 5px;"
                "    font-size: 14px;"
                "    color: #333;"
                "}"
                "QDateEdit::drop-down {"
                "    subcontrol-origin: padding;"
                "    subcontrol-position: top right;"
                "    width: 20px;"
                "    border-left: 2px solid #3498db;"
                "}"
                "QDateEdit:focus {"
                "    border-color: #2ecc71;" // Color of the border when it is focused
                "}"
            );
        }
    });
    //END valideDateNaiss

    //BEGIN valideEmail
    connect(ui->email, &QLineEdit::textChanged, this, [this]() {
        if(!valideEmail(ui->email->text())){
            showError(ui->email);
        }
        else {
            hideError(ui->email);
        }
    });
    //END valideEmail

    //BEGIN valideNumTel
    connect(ui->numtel, &QLineEdit::textChanged, this, [this]() {
        if(!valideTEL(ui->numtel->text())){
            showError(ui->numtel);
        }
        else {
            hideError(ui->numtel);
        }
    });
    //END valideNumTel

    //BEGIN valideAdesse
    connect(ui->adresse, &QLineEdit::textChanged, this, [this]() {
        if(ui->adresse->text().isEmpty() || ui->adresse->text().length()<3 ){
            showError(ui->adresse);
        }
        else {
            hideError(ui->adresse);
        }
    });
    //END valideAdesse

    //BEGIN valideSalaire
    connect(ui->salaire, &QLineEdit::textChanged, this, [this]() {
        if(!valideSalaire(ui->salaire->text())){
            showError(ui->salaire);
        }
        else {
            hideError(ui->salaire);
        }
    });
    //END valideSalaire

    /*
    //BEGIN valideMdp
    connect(ui->mdp, &QLineEdit::textChanged, this, [this]() {
        if(!valideMdp(ui->mdp->text())){
            showError(ui->mdp);
        }
        else {
            hideError(ui->mdp);
        }
    });
    //END valideMdp
    */

    //BEGIN logout
    connect(ui->logout, &QPushButton::clicked, this, &Gestion_Personnel::logout);
    //END logout

    //BEGIN Modifier_Employee
        connect(ui->tableW, &QTableWidget::itemChanged, [=](QTableWidgetItem *item){
            int modifiedRow = item->row();
            int modifiedColumn = item->column();
            QTableWidgetItem *headerItem = ui->tableW->horizontalHeaderItem(modifiedColumn);
            QString columnHeader = headerItem->text();
            QWidget *widget = ui->tableW->cellWidget(modifiedRow, 11);
            QPushButton *button = qobject_cast<QPushButton*>(widget);
            employee E;
            if(ui->OnOffModification->isChecked()){
                if(E.Modifier_Employee(item,columnHeader,button->objectName().toInt())){
                    QMessageBox::information(this, "Succès", "Employé MODIFIE avec succès.");
                }
                else{
                    QMessageBox::critical(this, "Erreur", "Erreur.");
                }
                ui->OnOffModification->setCheckState(Qt::Unchecked);
            }
        });
    //END Modifier_Employee

    //BEGIN Tasks
        connect(ui->chat, &QPushButton::clicked, this, &Gestion_Personnel::tasks);
    //END Tasks

}
//END CONSTRUCTOR

//BEGIN DESTRUCTOR
Gestion_Personnel::~Gestion_Personnel()
{
    delete ui;
}
//END DESTRUCTOR

//BEGIN LOGOUT
void Gestion_Personnel::logout(){
    MainWindow* w = new MainWindow();
    w->show();
    this->close();
}
//END LOGOUT

//BEGIN filterTable
void Gestion_Personnel::filterTable(const QString &text) {
    for (int row = 0; row < ui->tableW->rowCount(); ++row) {
        bool match = false;
        for (int col = 0; col < ui->tableW->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableW->item(row, col);
            if (item && item->text().contains(text, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        ui->tableW->setRowHidden(row, !match);
    }
}
//END filterTable

//BEGIN detectAfficher
void Gestion_Personnel::detectAfficher(int code){
    QSqlQueryModel* m = new QSqlQueryModel();
    if(code==0){
        m=employee::afficher_Employees();
    }
    else if(code==1){
        m->setQuery("SELECT id_emp,CIN,nom, prenom,TO_CHAR(datenaiss, 'YYYY-MM-DD' ) as datenaiss,TO_CHAR(dateEmb, 'YYYY-MM-DD' ) as dateEmb,email,sexe, adresse, numtel, salaire,Role FROM employee order by salaire;");
    }
    else if(code==2){
        m->setQuery("SELECT id_emp,CIN,nom, prenom,TO_CHAR(datenaiss, 'YYYY-MM-DD' ) as datenaiss,TO_CHAR(dateEmb, 'YYYY-MM-DD' ) as dateEmb,email,sexe, adresse, numtel, salaire,Role FROM employee order by salaire desc;");
    }
    else if(code==3){
        m->setQuery("SELECT id_emp,CIN,nom, prenom,TO_CHAR(datenaiss, 'YYYY-MM-DD' ) as datenaiss,TO_CHAR(dateEmb, 'YYYY-MM-DD' ) as dateEmb,email,sexe, adresse, numtel, salaire,Role FROM employee order by datenaiss ;");
    }
    else if(code==4){
        m->setQuery("SELECT id_emp,CIN,nom, prenom,TO_CHAR(datenaiss, 'YYYY-MM-DD' ) as datenaiss,TO_CHAR(dateEmb, 'YYYY-MM-DD' ) as dateEmb,email,sexe, adresse, numtel, salaire,Role FROM employee order by datenaiss desc;");
    }
    else if(code==5){
        m->setQuery("SELECT id_emp,CIN,nom, prenom,TO_CHAR(datenaiss, 'YYYY-MM-DD' ) as datenaiss,TO_CHAR(dateEmb, 'YYYY-MM-DD' ) as dateEmb,email,sexe, adresse, numtel, salaire,Role FROM employee order by nom ;");
    }
    else if(code==6){
        m->setQuery("SELECT id_emp,CIN,nom, prenom,TO_CHAR(datenaiss, 'YYYY-MM-DD' ) as datenaiss,TO_CHAR(dateEmb, 'YYYY-MM-DD' ) as dateEmb,email,sexe, adresse, numtel, salaire,Role FROM employee order by nom desc;");
    }
    afficherPersonnels(m);
}
//END detectAfficher

//BEGIN afficherPersonnels
void Gestion_Personnel::afficherPersonnels(QSqlQueryModel* m){
    ui->tableW->setRowCount(m->rowCount());
    ui->tableW->setColumnCount(m->columnCount());
    QStringList headers;
    for (int col = 1; col < m->columnCount(); ++col) {
        headers << m->headerData(col, Qt::Horizontal).toString();
    }
    headers << "Action1" << "Action2";
    ui->tableW->setHorizontalHeaderLabels(headers);
    for (int row = 0; row < m->rowCount(); ++row) {
        for (int col = 1; col < m->columnCount(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(m->data(m->index(row, col)).toString());
            ui->tableW->setItem(row, col-1, item);
        }

        QPushButton *deleteButton = new QPushButton("supprimer", ui->tableW);
        deleteButton->setStyleSheet("QPushButton {\
                                        padding: 5px;\
                                        font-size: 16px;\
                                        text-align: center;\
                                        text-decoration: none;\
                                        border: 2px solid #792382;\
                                        color: #792382;\
                                        border-radius: 5px;\
                                        background-image: url('D:/ESPRIT/c++/Projet/untitled/images/trash.svg');\
                                        background-repeat: no-repeat;\
                                        background-position: left center;\
                                        padding-left: 30px; \
                                    }\
                                    QPushButton:hover {\
                                        background-color: #43aff7;\
                                    }");
        ui->tableW->setCellWidget(row, m->columnCount()-1, deleteButton);
        connect(deleteButton, &QPushButton::clicked, this, &Gestion_Personnel::DeleteEmployee);
        int employeeId = m->data(m->index(row, 0)).toInt();
        deleteButton->setObjectName(QString::number(employeeId));

    }

    QLayout *existingLayout = layout();
    if (existingLayout) {

        QVBoxLayout *verticalLayout = qobject_cast<QVBoxLayout*>(existingLayout);
        if (verticalLayout) {
            verticalLayout->addWidget(ui->tableW);
        }
    }
}
//END afficherPersonnels

//BEGIN DeleteEmployee
void Gestion_Personnel::DeleteEmployee()
{
    QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
    if (!senderButton)
        return;
    int idEmp = senderButton->objectName().toInt();
    QMessageBox::StandardButton confirmation = QMessageBox::question(this, "Confirmation",
                                                                             "Êtes-vous sûr de vouloir supprimer cet employé ?",
                                                                             QMessageBox::Yes | QMessageBox::No);
    if (confirmation == QMessageBox::Yes) {
        employee e;
        bool success = e.Effacer_Employee(idEmp);
        if (success) {
            QMessageBox::information(this, "Succès", "Employé supprimé avec succès.");
            detectAfficher(0);
        } else {
            QMessageBox::critical(this, "Erreur", "Impossible de supprimer l'employé.");
        }
    }
}
//END DeleteEmployee

//BEGIN ajouterPersonnel
void Gestion_Personnel::ajouterPersonnel(){
    QString CIN=ui->CIN->text();
    QString nom = ui->nom->text();
    QString prenom = ui->prenom->text();
    QString email = ui->email->text();
    QString adresse = ui->adresse->text();
    QString sexe = "";
    if(ui->H->isChecked()) {
        sexe = "H";
    } else if(ui->F->isChecked()) {
        sexe = "F";
    }

    QString Role = ui->Role->currentText();
    forgetP F;
    QString mdp = F.generateVerificationCode();
    int num_tel = ui->numtel->text().toInt();
    QDate age = ui->age->date();
    float salaire = ui->salaire->text().toFloat();

    if(!valideCIN(CIN) ||
       !valideTEL(ui->numtel->text()) ||
       !valideEmail(email) ||
       !valideDateNaiss(age) ||
       !valideSalaire(ui->salaire->text())||
       ui->nom->text().isEmpty() || ui->nom->text().length()<3 ||
       ui->prenom->text().isEmpty() || ui->prenom->text().length()<3 ||
       ui->adresse->text().isEmpty() || ui->adresse->text().length()<3
      )
    {
        QMessageBox::critical(this, "Erreur", "Il y a des champs non valides!");
        return;
     }

    if(ui->Role->currentText().isEmpty()){
        ui->Role->setStyleSheet(
            "padding: 5px;"
            "border: 2px solid red;"
            "border-radius: 5px;"
            "font-size: 14px;"
            "color: #333;"
        );
        return;
    }
    else{
        ui->Role->setStyleSheet(
            "padding: 5px;"
            "border: 2px solid #43aff7;"
            "border-radius: 5px;"
            "font-size: 14px;"
            "color: #333;"
        );
    }

    employee E(0, nom, prenom, age, email, Role, num_tel, adresse, sexe, mdp, salaire,CIN,age);
    if(E.Existance_Employee(email) || E.Existance_Employee_CIN(CIN)){
        QMessageBox::critical(nullptr, QObject::tr("system"),QObject::tr("Employee Exist.\n"));
    }
    else{
        bool test = E.Ajouter_Employee();

        if (test) {
            QStringList arguments;
            arguments << "C:/Users/azizz/Downloads/ProjetC++/PythonScripts/mail2.py" <<email<<mdp;

            QProcess::startDetached("python", arguments);
            QMessageBox::information(nullptr, QObject::tr("system"),QObject::tr("Employee ajoute avec succes.\n"));
            detectAfficher(0);
        } else {
            QMessageBox::information(nullptr, QObject::tr("system"),QObject::tr("Erreur.\n"));
        }
    }


    ui->CIN->clear();
    ui->nom->clear();
    ui->prenom->clear();
    ui->email->clear();
    ui->numtel->clear();
    ui->salaire->clear();
    ui->adresse->clear();
    hideError(ui->CIN);
    hideError(ui->nom);
    hideError(ui->prenom);
    hideError(ui->email);
    hideError(ui->numtel);
    hideError(ui->salaire);
    hideError(ui->adresse);
    ui->Role->setStyleSheet(
        "padding: 5px;"
        "border: 2px solid #43aff7;"
        "border-radius: 5px;"
        "font-size: 14px;"
        "color: #333;"
    );
    ui->Role->setCurrentIndex(0);
    ui->CIN->setPlaceholderText("CIN");
    ui->nom->setPlaceholderText("Nom");
    ui->prenom->setPlaceholderText("Prenom");
    ui->email->setPlaceholderText("Email");
    ui->numtel->setPlaceholderText("Numero de telephone");
    ui->salaire->setPlaceholderText("Salaire");
    ui->adresse->setPlaceholderText("Adresse");

}
//END ajouterPersonnel

//BEGIN valideEmail
bool Gestion_Personnel::valideEmail(QString email) {
    QRegularExpression regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");

    QRegularExpressionMatch match = regex.match(email);

    if (match.hasMatch()) {
        return true;
    } else {
        return false;
    }
}
//END valideEmail

//BEGIN valideCIN
bool Gestion_Personnel::valideCIN(QString CIN){
    QRegularExpression regex("^[0-9]{8}$");
        if (regex.match(CIN).hasMatch()) {
            return true;
        } else {
            return false;
        }
}
//END valideCIN

//BEGIN valideTEL
bool Gestion_Personnel::valideTEL(QString tel) {

    QRegularExpression regex("^\\d{8}$");

    QRegularExpressionMatch match = regex.match(tel);

    if (match.hasMatch()) {
        return true;
    } else {
        return false;
    }
}
//END valideTEL

//BEGIN valideSalaire
bool Gestion_Personnel::valideSalaire(QString salaire) {

    bool ok;
    float salaireValue = salaire.toFloat(&ok);

    if (ok && salaireValue > 0) {
        return true;
    } else {
        return false;
    }
}
//END valideSalaire

//BEGIN valideMdp
bool Gestion_Personnel::valideMdp(QString password) {

    bool hasLowercase = false;
    bool hasNumber = false;
    bool hasSpecialChar = false;

    for (const QChar& c : password) {
        if (c.isLower()) {
            hasLowercase = true;
        } else if (c.isDigit()) {
            hasNumber = true;
        } else if (!c.isLetterOrNumber()) {
            hasSpecialChar = true;
        }
    }

    if (password.length() >= 8 && hasLowercase && hasNumber && hasSpecialChar) {
        return true;
    } else {
        return false;
    }
}
//END valideMdp

//BEGIN valideDateNaiss
bool Gestion_Personnel::valideDateNaiss(QDate dateNaiss) {

    QDate currentDate = QDate::currentDate();

    int age = dateNaiss.daysTo(currentDate) / 365;

    if (age >= 18 && age <= 100) {
        return true;
    } else {
        return false;
    }
}
//END valideDateNaiss

//BEGIN valideDate
bool Gestion_Personnel::valideDate(QDate DateEmb) {
    if (!DateEmb.isValid()) {
        return false;
        }
    return true;
}
//END valideDate

//BEGIN showError
void Gestion_Personnel::showError(QLineEdit* EditLine){
    QString styleSheet = "QLineEdit {"
                                         "    padding: 5px;"
                                         "    border: 2px solid red;"
                                         "    border-radius: 5px;"
                                         "    font-size: 14px;"
                                         "    color: #333;"
                                         "}"
                                         "QLineEdit::placeholder {"
                                         "    color: red;"
                                         "}";
    EditLine->setStyleSheet(styleSheet);
    EditLine->setPlaceholderText("non valide!");
    EditLine->setFocus();
}
//END showError

//BEGIN hideError
void Gestion_Personnel::hideError(QLineEdit* EditLine){
    EditLine->setStyleSheet("QLineEdit {"
                                            "padding: 5px;"
                                            "    border: 2px solid #43aff7;"
                                            "    border-radius: 5px;"
                                            "    font-size: 14px;"
                                            "    color: #333;"
                                            "}");
}
//END hideError

//BEGIN downloadPDF
void Gestion_Personnel::downloadPDF() {

    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "", "*.pdf");
    if (fileName.isEmpty())
        return;

    QPdfWriter pdfWriter(fileName);
    QPainter painter(&pdfWriter);

    painter.drawText(1, 100, "Employee Details: ");

    int numRows = ui->tableW->rowCount();
    int numCols = ui->tableW->columnCount();
    int y = 200;
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            QTableWidgetItem *item = ui->tableW->item(row, col);
            if (item) {
                QString text = item->text();
                if(col==6){
                    painter.drawText( 400+col * 1000, y*(row+3), text);
                }
                else{
                    painter.drawText( col * 1000, y*(row+3), text);
                }

            }
        }
    }
    painter.end();
    QMessageBox::information(this, "PDF Downloaded", "PDF has been downloaded successfully.");
}
//END downloadPDF

//BEGIN statistic
void Gestion_Personnel::statistic() {
    QSqlQueryModel* model = employee::afficher_Employees();

    if (!model) {
        qDebug() << "Error: Failed to retrieve employee data.";
        return;
    }

    int rowCount = model->rowCount();

    int H = 0, F = 0, GP = 0, GPA = 0, GE = 0, GO = 0, GC = 0, E = 0;
    for (int i = 0; i < rowCount; ++i) {
        if (model->data(model->index(i, 7)).toString() == "H") {
            H++;
        } else if (model->data(model->index(i, 7)).toString() == "F") {
            F++;
        }

        if (model->data(model->index(i, 11)).toString() == "Employee") {
            E++;
        } else if (model->data(model->index(i, 11)).toString() == "Gestion_Personnels") {
            GP++;
        } else if (model->data(model->index(i, 11)).toString() == "Gestion_Oeuvres") {
            GO++;
        } else if (model->data(model->index(i, 11)).toString() == "Gestion_Participants") {
            GPA++;
        } else if (model->data(model->index(i, 11)).toString() == "Gestion_Charites") {
            GC++;
        } else {
            GE++;
        }
    }

    float PF = (F * 100.0) / rowCount;
    float PH = (H * 100.0) / rowCount;
    float PE = (E * 100.0) / rowCount;
    float PGP = (GP * 100.0) / rowCount;
    float PGO = (GO * 100.0) / rowCount;
    float PGPA = (GPA * 100.0) / rowCount;
    float PGC = (GC * 100.0) / rowCount;
    float PGE = (GE * 100.0) / rowCount;

    // Create a bar series
    QtCharts::QBarSeries *series = new QtCharts::QBarSeries();

    QtCharts::QBarSet *roleSet = new QtCharts::QBarSet("Role");
    *roleSet << PF << PH << PE << PGP << PGO << PGPA << PGC << PGE;
    series->append(roleSet);

    // Create a chart and add the series
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Employee Statistics");

    // Create a category axis and set labels
    QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
    axis->append("Female");
    axis->append("Male");
    axis->append("Employee");
    axis->append("Gestion_Personnels");
    axis->append("Gestion_Oeuvres");
    axis->append("Gestion_Participants");
    axis->append("Gestion_Charites");
    axis->append("Gestion_Evenements");

    chart->setAxisX(axis, series);

    // Create a value axis and set the range
    QtCharts::QValueAxis *valueAxis = new QtCharts::QValueAxis();
    valueAxis->setRange(0, 100);
    chart->setAxisY(valueAxis, series);
    ui->widget->raise();
    ui->statOFF->raise();

    // Add legend
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Display the chart
    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setFixedSize(1600, 600);

    QVBoxLayout *layout = new QVBoxLayout(ui->widget);
    layout->addWidget(chartView);
    ui->widget->setLayout(layout);

    // Clean up
    delete model;
}

//END statistic

//BEGIN statisticOFF
void Gestion_Personnel::statisticOFF(){
    ui->widget->lower();
    ui->statOFF->lower();
}
//END statisticOFF

//BEGIN assistance
void Gestion_Personnel::assistance() {
    QProcess process;

        // Set the command to execute the Python script directly
        QString pythonCommand = "python"; // Command to execute Python interpreter
        QStringList scriptArgs;
        scriptArgs << "C:\\Users\\bechi\\Desktop\\ProjetC++\\speechRecognition.py"; // Replace with the absolute path to your Python script

        // Start the process with the specified command and arguments
        process.start(pythonCommand, scriptArgs);

        // Wait for the process to finish
        process.waitForFinished(-1);
        QString result = process.readAllStandardOutput();
        ui->commands->setText(result);

    Gestion_Personnel g;
    QTextToSpeech *tts = new QTextToSpeech(this);
    QString employeeID;



    QRegularExpression idRegex("\\b\\d+\\b"); // Matches a sequence of digits

    QStringList words = result.split(" ");
    QString text = result.replace(" ","");
    text=text.replace(",","");
    text=text.replace("$","");
    //QStringList words1 = result.split(" ", QString::SkipEmptyParts);

    if(result.contains("exit")){
        QTextToSpeech *tts = new QTextToSpeech(this);
        QString text = "Hope that i helped you ";
        tts->say(text);
        ui->commands->lower();
        ui->confirm->lower();
    }
    else if (words.contains("delete") && words.contains("employee")) {
        for (const QString& word : words) {
               if (idRegex.match(word).hasMatch()) {
                   employeeID = word;
                   break;
               }
           }
        employee E;
        if(E.Existance_Employee_ID(employeeID.toInt())){
            if(E.Effacer_Employee(employeeID.toInt())){
                tts->say("Employee Deleted successfully");
                detectAfficher(0);
            }
            else {
                tts->say("An Error occured while deleting the Employee");
            }
        }
        else{
            tts->say("this Employee does not Exist");
        }
    }
    else if (words.contains("add") && words.contains("employee")) {
        // Extracting information for adding an employee
        QString Nom, Prenom, Email, Adresse, Sexe,sex, Mdp, Role,CIN,NumTel,Salaire,age;
        QDate Age,DateEmb;
        //if (index + 14 <= words.size()) {
            // Initialize variables with extracted values
            Nom = words[words.indexOf("name") + 1];
            Prenom = words[words.lastIndexOf("name") + 1];
            int emailIndex = words.indexOf("email");
            int genderIndex = words.indexOf("gender");
            QStringList emailWords = words.mid(emailIndex + 1, genderIndex - emailIndex - 1);
                // Concatenate the words with a space between them to form the email address
                Email = emailWords.join(" ");
                // Replace placeholders in the email address
                Email.replace(" at ", "@");
                Email.replace(" dot ", ".");
                Email.replace(" ", "");
            sex=words[words.indexOf("gender") + 1][0].toUpper();
            if(sex=="M"){
                Sexe="H";
            }
            else{
                Sexe="F";
            }
            Adresse = words[words.indexOf("address")+1];
            if(text.indexOf("cin")!=-1){
                for (int i=text.indexOf("cin");i<text.length();i++){
                    if( text.at(i).isDigit() && CIN.length()<8 ){
                        CIN+=text.at(i);
                    }
                }
            }
            if(text.indexOf("phonenumber")!=-1){
                for (int i=text.indexOf("phonenumber");i<text.length();i++){
                    if( text.at(i).isDigit() && NumTel.length()<8 ){
                        NumTel+=text.at(i);
                    }
                }
            }
            if(text.indexOf("birthdate")!=-1){
                for (int i=text.indexOf("birthdate");i<text.length();i++){
                    if( (text.at(i).isDigit() || text.at(i)=="-") && NumTel.length()<10 ){
                        age+=text.at(i);
                    }
                }
            }
            age=age.replace("Dash","-");
            Age=QDate::fromString(age, "yyyy-MM-dd");
            Salaire = words[words.indexOf("salary")+1];
            employee E(0, Nom, Prenom, Age, Email, Role, NumTel.toInt(), Adresse, Sexe, Mdp, Salaire.toFloat(), CIN, DateEmb);
            if (E.Existance_Employee(Email) || E.Existance_Employee_CIN(CIN)) {
                tts->say("This employee already exists.");
            } else {
                // Attempt to add the employee
                bool success = E.Ajouter_Employee();
                if (success) {
                    tts->say("Employee added successfully.");
                    detectAfficher(0);
                } else {
                    tts->say("An error occurred while adding the employee.");
                }
            }
        //} else {
           // qDebug() << "Insufficient information to add employee.";
        //}

    }
    else if(words.contains("download") ){
        ui->downloadPDF->click();
    }
    else if(result!="exit") {
        tts->say("that's an Invalid command: ");
    }
}
//END assistance

//BEGIN Tasks
void Gestion_Personnel::tasks(){
    QTextToSpeech *tts = new QTextToSpeech(this);
    QString text = "Hi ,how can i assist you ?";
    tts->say(text);
    ui->commands->raise();
    ui->confirm->raise();
}
//END Tasks

//BEGIN scan_CIN
void Gestion_Personnel::scan_CIN(){
    QProcess process;

    QString pythonCommand = "python"; // Command to execute Python interpreter
    QStringList scriptArgs;
    //scriptArgs << "C:\\Users\\bechi\\Desktop\\ProjetC++\\PythonScripts\\cinScan.py";
    scriptArgs << "C:/Users/azizz/Downloads/ProjetC++/PythonScripts/cinScan.py";


    process.start(pythonCommand, scriptArgs);

    process.waitForFinished(-1);
    QString result = process.readAllStandardOutput();
    ui->CIN->setText(result);
    result=ui->CIN->text();
    result = result.left(result.length() - 2);
    ui->CIN->setText(result);
}
//END scan_CIN

//BEGIN scan_Face
void Gestion_Personnel::scan_Face(){
    QString role=ui->Role->currentText();
    if(!role.isEmpty()){
        QString pythonCommand = "python"; // Command to execute Python interpreter
        QStringList scriptArgs;

        scriptArgs << "C:/Users/azizz/Downloads/ProjetC++/PythonScripts/face/register.py"<<role;


        QProcess process;

        process.start(pythonCommand, scriptArgs);

        process.waitForFinished(-1);
    }
    else{
        QMessageBox::information(nullptr, QObject::tr("system"),QObject::tr("Role est vide!!"));
    }




}
//END scan_Face
