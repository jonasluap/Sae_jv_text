
// LUAP Jonas | EL BAZTAMI Manal

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE 256
#define MAX_PATH 100
#define SAVE_FILE "sauvegarde.txt"

// Structure de sauvegarde
typedef struct {
    char nom[20];
    char langue[10];
    int branche;        // 0: début, 1: injection, 2: dilution
    int sous_branche;   // 0-2 selon le choix
    int choix_precedent; // dernier choix fait
    int etape_carte;    // étape actuelle de la carte
} Sauvegarde;

// Function that displays a text file line by line
void afficher_fichier(const char *nom_fichier)
{
    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier == NULL)
    {
        printf("Error: cannot open %s\n", nom_fichier);
        return;
    }

    char ligne[MAX_LINE];
    
    printf("\n\n");

    while (fgets(ligne, sizeof(ligne), fichier) != NULL)
    {
        printf("%s", ligne);
    }

    fclose(fichier);
}

// Function to convert to lowercase
void mettre_en_minuscule(char *chaine)
{
    for (int i = 0; chaine[i]; i++)
    {
        chaine[i] = tolower(chaine[i]);
    }
}

// Function to clear input buffer
void vider_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to ask user for a choice with error handling
int demander_choix(const char *mot_choix, int choix_min, int choix_max)
{
    int choix;
    int valide;
    
    do {
        printf("\n%s (%d-%d): ", mot_choix, choix_min, choix_max);
        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            choix = -1;
        }
        vider_buffer();
        
        valide = (choix >= choix_min && choix <= choix_max);
        if (!valide) {
            printf("Invalid choice. Please enter a number between %d and %d.\n\n", choix_min, choix_max);
        }
    } while (!valide);
    
    return choix;
}

// Function to display a story step with optional wait
void afficher_etape(const char *dossier, const char *fichier_base, const char *langue, int avec_attente)
{
    char chemin[MAX_PATH];
    snprintf(chemin, sizeof(chemin), "%s/%s_%s.txt", dossier, fichier_base, langue);
    afficher_fichier(chemin);
    
    if (avec_attente) {
        printf("\n%s", strcmp(langue, "gb") == 0 ? "Press Enter to continue..." : "Appuyez sur Entree pour continuer...");
        getchar();
    }
}

// Function to display progressive map from files
void afficher_carte_progressive(const char *langue, int etape_id)
{
    char chemin[MAX_PATH];
    char nom_fichier[50];
    
    snprintf(nom_fichier, sizeof(nom_fichier), "carte_etape%d_%s.txt", etape_id, langue);
    snprintf(chemin, sizeof(chemin), "./cartes/%s", nom_fichier);
    
    FILE *fichier = fopen(chemin, "r");
    
    if (fichier == NULL && strcmp(langue, "fr") != 0) {
        snprintf(nom_fichier, sizeof(nom_fichier), "carte_etape%d_fr.txt", etape_id);
        snprintf(chemin, sizeof(chemin), "./cartes/%s", nom_fichier);
        fichier = fopen(chemin, "r");
    }
    
    if (fichier == NULL) {
        snprintf(chemin, sizeof(chemin), "./cartes/carte_etape%d.txt", etape_id);
        fichier = fopen(chemin, "r");
    }
    
    if (fichier == NULL) {
        // Si le fichier n'existe pas, chercher le fichier français
        snprintf(nom_fichier, sizeof(nom_fichier), "carte_etape%d_fr.txt", etape_id);
        snprintf(chemin, sizeof(chemin), "./cartes/%s", nom_fichier);
        fichier = fopen(chemin, "r");
    }
    
    if (fichier == NULL) {
        // Pas de message si fichier n'existe pas - carte optionnelle
        return;
    }
    
    char ligne[MAX_LINE];
    printf("\n");
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        printf("%s", ligne);
    }
    
    fclose(fichier);
}

// Function to save game
void sauvegarder_partie(const Sauvegarde *sauv)
{
    FILE *fichier = fopen(SAVE_FILE, "w");
    if (fichier == NULL) {
        return;
    }
    
    fprintf(fichier, "%s\n%s\n%d\n%d\n%d\n%d\n", 
            sauv->nom, sauv->langue, sauv->branche, 
            sauv->sous_branche, sauv->choix_precedent, sauv->etape_carte);
    
    fclose(fichier);
}

// Function to load saved game
int charger_sauvegarde(Sauvegarde *sauv)
{
    FILE *fichier = fopen(SAVE_FILE, "r");
    if (fichier == NULL) {
        return 0;
    }
    
    if (fgets(sauv->nom, sizeof(sauv->nom), fichier) == NULL ||
        fgets(sauv->langue, sizeof(sauv->langue), fichier) == NULL) {
        fclose(fichier);
        return 0;
    }
    
    sauv->nom[strcspn(sauv->nom, "\n")] = 0;
    sauv->langue[strcspn(sauv->langue, "\n")] = 0;
    
    if (fscanf(fichier, "%d\n%d\n%d\n%d\n", &sauv->branche, &sauv->sous_branche, 
               &sauv->choix_precedent, &sauv->etape_carte) != 4) {
        fclose(fichier);
        return 0;
    }
    
    fclose(fichier);
    return 1;
}

// Random chance function
int chance(int percent)
{
    return (rand() % 100) < percent;
}

// Trap function for bad choices
void piege_mauvais_choix(const char *langue)
{
    int type = rand() % 3;

    if (type == 0) {
        printf("\n%s\n", strcmp(langue, "gb") == 0 ? 
               "⚠️ TRAP: The mouse scratches you! Press Enter..." : 
               "⚠️ PIEGE : La souris vous griffe ! Appuyez sur Entree...");
    } else if (type == 1) {
        printf("\n%s\n", strcmp(langue, "gb") == 0 ? 
               "🚨 FIRE ALARM! Press Enter..." : 
               "🚨 ALARME INCENDIE ! Appuyez sur Entree...");
    } else {
        printf("\n%s\n", strcmp(langue, "gb") == 0 ? 
               "☠️ POISON! You cough, but you continue. Press Enter..." : 
               "☠️ POISON ! Vous toussez, mais vous continuez. Appuyez sur Entree...");
    }

    getchar();
}

// Function to display personalized message
void afficher_message_personnalise(const char *message_fr, const char *message_gb, const char *langue)
{
    printf("\n%s\n", strcmp(langue, "gb") == 0 ? message_gb : message_fr);
}

// Function to display end game screen
void afficher_fin_partie(const char *nom, const char *langue)
{
    printf("\n═══════════════════════════════════════\n");
    remove(SAVE_FILE);
    printf("\n═══════════════════════════════════════\n");
    printf("%s, %s!\n", strcmp(langue, "gb") == 0 ? "Thank you for playing" : "Merci d'avoir joué", nom);
    printf("%s\n", strcmp(langue, "gb") == 0 ? "Story completed!" : "Histoire terminée !");
    printf("═══════════════════════════════════════\n");
}

int main(void)
{
    srand((unsigned)time(NULL));

    char nom[20];
    char langue[10];
    char *choix;
    int choix_utilisateur;
    int etape_carte = 0;
    int branche = 0;
    int sous_branche = 0;
    
    Sauvegarde sauvegarde;

    // Check for existing save file
    if (charger_sauvegarde(&sauvegarde)) {
        printf("\n═══════════════════════════════════════\n");
        if (strcmp(sauvegarde.langue, "gb") == 0) {
            printf("Saved game found for: %s\n", sauvegarde.nom);
            printf("Press Enter to continue or 'N' for new game: ");
        } else {
            printf("Sauvegarde trouvee pour : %s\n", sauvegarde.nom);
            printf("Appuyez sur Entree pour continuer ou 'N' pour nouvelle partie : ");
        }
        printf("═══════════════════════════════════════\n");
        
        // Read user response
        char reponse[10];
        if (fgets(reponse, sizeof(reponse), stdin) != NULL) {
            reponse[strcspn(reponse, "\n")] = 0;
            mettre_en_minuscule(reponse);
            
            // User wants new game
            if (strlen(reponse) > 0 && (reponse[0] == 'n' || strcmp(reponse, "nouveau") == 0 || strcmp(reponse, "new") == 0)) {
                printf("\n%s\n", strcmp(sauvegarde.langue, "gb") == 0 ? 
                       "Starting new game..." : "Début d'une nouvelle partie...");
                remove(SAVE_FILE);
                // Reset for new game
                branche = 0;
                sous_branche = 0;
                etape_carte = 0;
                // Ask for language again
                goto nouvelle_partie;
            }
        }
        
        // Load saved game data
        strcpy(nom, sauvegarde.nom);
        strcpy(langue, sauvegarde.langue);
        branche = sauvegarde.branche;
        sous_branche = sauvegarde.sous_branche;
        etape_carte = sauvegarde.etape_carte;
        
        choix = strcmp(langue, "gb") == 0 ? "choice" : "choix";
        printf("\n%s, %s!\n", strcmp(langue, "gb") == 0 ? "Welcome back" : "Bienvenue de retour", nom);
        
        // Display story without map for name entry
        goto suite_apres_chargement;
    }

nouvelle_partie:
    // Start new game - language selection
    do {
        printf("Choisir la langue (fr) / Select language (gb): ");
        if (scanf("%9s", langue) != 1) {
            vider_buffer();
            continue;
        }
        vider_buffer();
        mettre_en_minuscule(langue);
    } while (strcmp(langue, "fr") != 0 && strcmp(langue, "gb") != 0);

    choix = strcmp(langue, "gb") == 0 ? "choice" : "choix";
    
    // Display introduction without map
    afficher_etape("./histoire", "1intro", langue, 0);
    
    // Get player name (no map displayed here)
    do {
        printf("\n%s: ", strcmp(langue, "gb") == 0 ? "Enter your name" : "Entrez votre nom");
        if (!fgets(nom, sizeof(nom), stdin)) {
            vider_buffer();
            continue;
        }
        nom[strcspn(nom, "\n")] = 0;
    } while (strlen(nom) == 0);

suite_apres_chargement:
    // Update save data
    strcpy(sauvegarde.nom, nom);
    strcpy(sauvegarde.langue, langue);
    sauvegarde.branche = branche;
    sauvegarde.sous_branche = sous_branche;
    sauvegarde.etape_carte = etape_carte;
    sauvegarde.choix_precedent = 0;
    sauvegarder_partie(&sauvegarde);

    // Story beginning or continuation
    if (branche == 0 && sous_branche == 0) {
        afficher_etape("./histoire", "2_0_debut", langue, 0);
        
        // Display map BEFORE choice
        afficher_carte_progressive(langue, etape_carte);
        
        choix_utilisateur = demander_choix(choix, 1, 2);
        sauvegarde.choix_precedent = choix_utilisateur;
        
        // Update map step and display new map
        etape_carte = (choix_utilisateur == 1) ? 1 : 3; // Étape 1 pour injection, 3 pour dilution
        sauvegarde.etape_carte = etape_carte;
        
        branche = (choix_utilisateur == 1) ? 1 : 2;
        sauvegarde.branche = branche;
        sauvegarder_partie(&sauvegarde);
    }
    
    // Branch 1 - Immediate injection
    if (branche == 1) {
        if (sous_branche == 0) {
            afficher_etape("./histoire", strcmp(langue, "gb") == 0 ? "2_1_Inject_immediately" : "2_1_Injecter_immédiatement_une_dose", langue, 0);
            
            // Display map BEFORE choice
            afficher_carte_progressive(langue, etape_carte);
            
            choix_utilisateur = demander_choix(choix, 1, 2);
            sauvegarde.choix_precedent = choix_utilisateur;
            
            // Update map step and display new map
            etape_carte = (choix_utilisateur == 1) ? 2 : 4; // Étape 2 pour garder, 4 pour informer
            sauvegarde.etape_carte = etape_carte;
            
            sous_branche = (choix_utilisateur == 1) ? 1 : 2;
            sauvegarde.sous_branche = sous_branche;
            sauvegarder_partie(&sauvegarde);
            
            // Display new map after choice
            printf("\n");
            afficher_carte_progressive(langue, etape_carte);
        }
        
        if (sous_branche == 1) {
            afficher_etape("./histoire", strcmp(langue, "gb") == 0 ? "3_1_1_Treatment_room" : "3_1_1_Dans_la_salle_de_traitement", langue, 0);
            
            // Display map BEFORE choice
            afficher_carte_progressive(langue, etape_carte);
            
            choix_utilisateur = demander_choix(choix, 1, 2);
            sauvegarde.choix_precedent = choix_utilisateur;
            
            if (choix_utilisateur == 1) {
                // Update map step
                etape_carte = 3;
                sauvegarde.etape_carte = etape_carte;
                
                piege_mauvais_choix(langue);
                afficher_etape("./histoire", strcmp(langue, "gb") == 0 ? "4_1_keep_poison" : "4_1_garder_le_poison", langue, 0);
                
                // Display final map
                printf("\n");
                afficher_carte_progressive(langue, 5); // Carte de fin
                
                afficher_fin_partie(nom, langue);
            } else {
                afficher_etape("./histoire", strcmp(langue, "gb") == 0 ? "4_2_Inform_your_managers" : "4_2_Informer_vos_responsables", langue, 0);
                
                // Display map BEFORE choice
                afficher_carte_progressive(langue, etape_carte);
                
                choix_utilisateur = demander_choix(choix, 1, 2);
                sauvegarde.choix_precedent = choix_utilisateur;
                
                // Update map step
                etape_carte = (choix_utilisateur == 1) ? 5 : 6;
                sauvegarde.etape_carte = etape_carte;
                
                sauvegarder_partie(&sauvegarde);
                
                // Display new map after choice
                printf("\n");
                afficher_carte_progressive(langue, etape_carte);
                
                if (choix_utilisateur == 1) {
                    afficher_etape("./histoire", strcmp(langue, "gb") == 0 ? "5_2_1_Using_folders" : "5_2_1_Utiliser_les_dossiers", langue, 0);
                    afficher_fin_partie(nom, langue);
                } else {
                    piege_mauvais_choix(langue);
                    afficher_etape("./histoire", strcmp(langue, "gb") == 0 ? "5_2_2_Hide_information" : "5_2_2_Cacher_informations", langue, 0);
                    afficher_fin_partie(nom, langue);
                }
            }
        } else if (sous_branche == 2) {
            piege_mauvais_choix(langue);
            
            // Update map step
            etape_carte = 7;
            sauvegarde.etape_carte = etape_carte;
            
            // Display final map
            printf("\n");
            afficher_carte_progressive(langue, 7);
            
            afficher_etape("./histoire", strcmp(langue, "gb") == 0 ? "3_1_2_surveillance_room" : "3_1_2_salle_de_surveillance", langue, 0);
            afficher_fin_partie(nom, langue);
        }
    } 
    // Branch 2 - Dilution
    else if (branche == 2) {
        afficher_etape("./histoire", strcmp(langue, "gb") == 0 ? "2_2_dilute" : "2_2_diluer", langue, 0);
        
        // Display map BEFORE choice
        afficher_carte_progressive(langue, etape_carte);
        
        choix_utilisateur = demander_choix(choix, 1, 2);
        sauvegarde.choix_precedent = choix_utilisateur;
        
        // Update map step
        etape_carte = (choix_utilisateur == 1) ? 8 : 9;
        sauvegarde.etape_carte = etape_carte;
        
        sauvegarder_partie(&sauvegarde);
        
        // Display new map after choice
        printf("\n");
        afficher_carte_progressive(langue, etape_carte);
        
        if (choix_utilisateur == 1) {
            piege_mauvais_choix(langue);
            afficher_etape("./histoire", strcmp(langue, "gb") == 0 ? "3_2_1_validate_report" : "3_2_1_valider_le_rapport", langue, 0);
            afficher_fin_partie(nom, langue);
        } else {
            piege_mauvais_choix(langue);
            afficher_etape("./histoire", strcmp(langue, "gb") == 0 ? "3_2_2_observe" : "3_2_2_observer", langue, 0);
            afficher_fin_partie(nom, langue);
        }

        
    }

    return 0;
}
