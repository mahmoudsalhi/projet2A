#include "tasks.h"
#include "ui_tasks.h"

Tasks::Tasks(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tasks)
{
    ui->setupUi(this);
    QTextToSpeech *tts = new QTextToSpeech(this);
    QString text = "Hi ,how can i assist you ?";
    tts->say(text);

    //BEGIN statOFF
    connect(ui->confirm, &QPushButton::clicked, this, &Tasks::assistance);
    //END statOFF
}

Tasks::~Tasks()
{
    delete ui;
}

void Tasks::assistance() {
    Gestion_Personnel g;
    QTextToSpeech *tts = new QTextToSpeech(this);
    QString text = ui->commands->toPlainText().trimmed().toLower(); // Trim whitespace
    QString text1 = ui->commands->toPlainText().trimmed();

    QRegularExpression idRegex("\\b\\d+\\b"); // Matches a sequence of digits

    QStringList words = text.split(" ", QString::SkipEmptyParts);
    QStringList words1 = text1.split(" ", QString::SkipEmptyParts);
    QString employeeCIN;
    for (const QString& word : words) {
        if (idRegex.match(word).hasMatch()) {
            employeeCIN = word;
            break;
        }
    }

    if (words.contains("supprimer") && words.contains("employee") && !employeeCIN.isEmpty()) {
        employee E;
        if(E.Existance_Employee_CIN(employeeCIN)){
            if(employee::Effacer_Employee_CIN(employeeCIN.toInt())){
                tts->say("Employee Deleted successfully");
            }
            else {
                tts->say("An Error occured while deleting the Employee");
            }
        }
        else{
            tts->say("this Employee does not Exist");
        }
    }
    else if (words.contains("ajouter") && words.contains("employee")) {
        // Extracting information for adding an employee
        QString Nom, Prenom, Email, Adresse, Sexe, Mdp, Role,CIN,NumTel,Salaire;
        QDate Age, DateEmb;

        // Assuming the order of information is fixed
        int index = words.indexOf("add");
        if (index + 14 <= words.size()) {

            Nom = words1[words.indexOf("nom") + 1];  // Skipping "Nom"
            Prenom = words1[words.indexOf("prenom") + 1];  // Skipping "Prenom"
            Email = words1[words.indexOf("email") + 1];  // Skipping "Email"
            Adresse = words1[words.indexOf("adresse") + 1];  // Skipping "Adresse"
            Sexe = words1[words.indexOf("sexe") + 1].toUpper();  // Skipping "Sexe"
            Mdp = words1[words.indexOf("mdp") + 1];  // Skipping "Mdp"
            Role = words1[words.indexOf("role") + 1];  // Skipping "Role"
            Role.toUpper().left(1) + Role.mid(1).toLower();
            CIN = words1[words.indexOf("cin") + 1];  // Skipping "CIN"
            NumTel = words1[words.indexOf("numtel") + 1];  // Skipping "NumTel"
            Age = QDate::fromString(words[words.indexOf("naissance") + 1], "yyyy/MM/dd");  // Skipping "Age"
            if (!Age.isValid()) {
                tts->say("that's date is an Invalid");
            }
            DateEmb = QDate::fromString(words[words.indexOf("embauche") +1]);  // Skipping "DateEmb"
            Salaire = words[words.indexOf("salaire") + 1];  // Skipping "Salaire"

            employee E(0,Nom,Prenom,Age,Email,Role,NumTel.toInt(),Adresse,Sexe,Mdp,Salaire.toFloat(),CIN,DateEmb);
            if(!g.valideCIN(CIN) ||
               !g.valideTEL(NumTel) ||
               !g.valideEmail(Email) ||
                !g.valideMdp(Mdp) ||
               !g.valideDateNaiss(Age) ||
               !g.valideSalaire(Salaire)||
               Nom.isEmpty() || Nom.length()<3 ||
               Prenom.isEmpty() || Prenom.length()<3 ||
               Adresse.isEmpty() || Adresse.length()<3 ||
               (Role!="Employee" && Role!="Gestion_Personnels" && Role!="Gestion_Evenements" && Role!="Gestion_Charites" && Role!="Gestion_Participants" && Role!="Gestion_Oeuvres") ||
               (Sexe!="H" && Sexe!="F")
              )
            {
                tts->say("Error there is an invalid data !");
                return;

             }
            if(E.Existance_Employee(Email) || E.Existance_Employee_CIN(CIN)){
                tts->say("this Employee is already Exist");
            }
            else{
                bool test = E.Ajouter_Employee();

                if (test) {
                    tts->say("Employee added successfully");
                } else {
                    tts->say("An Error occured while adding the Employee");
                }
            }

        } else {
            qDebug() << "Insufficient information to add employee.";
        }
    }else {
        tts->say("that's an Invalid command: ");
    }
}
