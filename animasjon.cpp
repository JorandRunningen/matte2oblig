#include "animasjon.h"

Animation::Animation(int n, double c): 
    GraphDrawer{-1, 4.5, -5, 5},
    guitarString{n, c, function},
    speed{n, c, function},
    textInput{{350, 250}, 300, 50, "Navn"},
    functionList{{650, 50}, 300, 50, options}
    {
    t = 0;
    dt = 0.01;
    dx = pi/n;
    nIndex = n;
    add(textInput);
    textInput.setVisible(false);
    add(functionList);
    next_frame(); // draw_text får dropdownlist til å ikke fungere, dersom man bruker drawtext i samme frame som man adder dropdownlist
    updateOptions();
    currentFunction = "Ny";
}

// selve animasjonen
void Animation::playAninmation(){
    while (!should_close() && !quit){
        chooseFunction();
        userDrawString();
        guitarString.updateString(t);
        drawGraphPoint(guitarString.getString());
        info();
        commands();
        if (drawGrid){grid();}
        next_frame();
        if (play){t += dt;}
        
    }
    function.saveAllFunctions();
}


void Animation::commands(){
    // pause
    if (is_key_down(KeyboardKey::SPACE) && !keyDown){
        play = !play;
        keyDown = true;
    }
    // reset
    else if (is_key_down(KeyboardKey::R) && !keyDown){
        t = 0;
        keyDown = true;
    }
    // vise/skjule info
    else if (is_key_down(KeyboardKey::I) && !keyDown){
        showInfo = !showInfo;
        keyDown = true;
    }
    // redigere
    else if (is_key_down(KeyboardKey::E) && !keyDown){
        if (edit){
            function.fourier(guitarString, speed);
            guitarString.coefficients = function.getNewFunction();
        }
        edit = !edit;
        keyDown = true;
        t = 0;
    }
    // lagre funksjon
    else if (is_key_down(KeyboardKey::S) && !keyDown && !edit){
        saveFunction();
        keyDown = true;
    }
    // slette funksjon
    else if (is_key_down(KeyboardKey::D) && !keyDown && !edit){
        deleteFunction();
        keyDown = true;
    }
    // rutenett
    else if (is_key_down(KeyboardKey::G) && !keyDown){
        drawGrid = !drawGrid;
        keyDown = true;
    }
    // bytte mellom å redigere posisjon eller fart
    else if (is_key_down(KeyboardKey::K) && !keyDown  && edit){
        editSpeed = !editSpeed;
        keyDown = true;
    }

    // sørge for at man kan holde visse taster inne mer enn én iterajson, men at kommandoen likevel bare utføres én gang
    else if (!is_key_down(KeyboardKey::R) && 
             !is_key_down(KeyboardKey::SPACE) &&
             !is_key_down(KeyboardKey::I) &&
             !is_key_down(KeyboardKey::E) &&
             !is_key_down(KeyboardKey::S) &&
             !is_key_down(KeyboardKey::D) &&
             !is_key_down(KeyboardKey::G) &&
             !is_key_down(KeyboardKey::K)){
        keyDown = false;
    }
    // spole
    if (is_key_down(KeyboardKey::LEFT) && !edit){t -= dt;}
    if (is_key_down(KeyboardKey::RIGHT) && !edit){t += dt;}

    if (is_key_down(KeyboardKey::UP) && !edit){dt *= 1.1 ;}
    if (is_key_down(KeyboardKey::DOWN) && !edit){dt *= 1/1.1;}
    
    // avslutte
    if (is_key_down(KeyboardKey::Q)){
        quit = true;
    } 

    // nullstille
    if (is_key_down(KeyboardKey::Z) && edit){
        if (editSpeed){
            speed.coefficients = function.none();
            speed.updateString(0);
        }
        else{
            guitarString.coefficients = function.none();
            guitarString.updateString(0);
        }
    } 
}

// skrive ut informasjon til brukeren
void Animation::info(){
    if (showInfo){
        draw_text(Point(105, 50),  "I: skjul info", Color::black, 15);
        draw_text(Point(105, 70),  "R: reset", Color::black, 15);
        draw_text(Point(105, 90),  "Q: avslutte", Color::black, 15);
        draw_text(Point(105, 110), "Mellomrom: pause", Color::black, 15);
        draw_text(Point(105, 130), "Piltaster høyre/venstre: spole", Color::black, 15);
        draw_text(Point(105, 150), "Piltaster opp/ned: endre tidssteg", Color::black, 15);
        draw_text(Point(105, 170), "G: rutenett", Color::black, 15);
        if (edit){
            draw_text(Point(105, 190), "E: avslutt redigering", Color::black, 15);
            draw_text(Point(105, 210), "Z: nullstille funksjon", Color::black, 15);
            if (editSpeed){draw_text(Point(105, 230), "K: redigere initialposisjon", Color::black, 15);}
            else          {draw_text(Point(105, 230), "K: redigere initialfart", Color::black, 15);}

        }
        else {
            draw_text(Point(105, 190), "S: lagre funksjon", Color::black, 15);
            draw_text(Point(105, 210), "D: slette funksjon", Color::black, 15);
            draw_text(Point(105, 230), "E: redigere", Color::black, 15);
        }
    }
    
    else {
        draw_text(Point(100, 50),  "Trykk I for å vise info", Color::black, 20);
    }

    if (edit){
        draw_text(Point(400, 50), "Redigere initialposiosjon", Color::black, 20);
        draw_text(Point(400, 75), "Redigere initialfart", Color::red, 20);

        if (editSpeed){draw_rectangle({400, 78}, 155, 23, Color::white, Color::blue);}
        else          {draw_rectangle({400, 53}, 200, 23, Color::white, Color::blue);}
    }     
    
}

void Animation::userDrawString(){
    while (!should_close() && edit && !quit){
        if (is_left_mouse_button_down()){
            Point pixelPoint = get_mouse_coordinates();
            vector<double> mathPoint = pixelPointToMathPoint(pixelPoint);
            int index = static_cast<int>(mathPoint.at(0)/dx);
            
            // 10 puntker redigeres samtidig, det gir smudere tegning hva brukeren tegner fort
            for (int i = index-5; i < index + 5; i++){
                if (i > 0 && i < nIndex - 1){
                    if (editSpeed){speed.editPoint(i, mathPoint.at(1));}
                    else {guitarString.editPoint(i, mathPoint.at(1));}
                }
            }
        }
        
        if (editSpeed){drawGraphPoint(speed.getString(), Color::red);}
        else {drawGraphPoint(guitarString.getString());} 
        info();
        commands();
        if (drawGrid){grid();}
        next_frame();
    }
}

void Animation::saveFunction(){
    textInput.setVisible(true);
    next_frame();
    bool useableName = false; // hvis bruekren legger innn navn
    bool unuseablename = false; // hvis brukeren legger inn ugyldig navn
    while(!useableName && 
          !should_close() &&
          !is_key_down(KeyboardKey::TAB)){
        draw_text({340, 320}, "Trykk enter for å velge navn eller tab for å avbryte",
                  Color::black, 15);
        if (unuseablename){
            draw_text({380, 200}, "Navnet er alt brukt, velg noe annet", Color::red, 15);
        }
        
        if (is_key_down(KeyboardKey::ENTER)){
            useableName = function.addFunction(textInput.getText()); // returnerer true om gyldig navn 
            if (!useableName){unuseablename = true;}
            updateOptions();
        }
        next_frame();
    }
    textInput.setText("Navn");
    textInput.setVisible(false);
    currentFunction = "Ny"; // dersom currentFunction var ny gjør dette ingenting, mens hvis currentFunction ikke er Ny, kommer dette til å trigge chooseFunction
                                // som nullstiller funksjonen man er på nå 
    next_frame();
}

void Animation::updateOptions(){
    options.clear();
    for (int i = 0; i < function.allFunctionsName.size(); i++){
        options.push_back(function.allFunctionsName.at(i));
    }
    options.push_back("Ny"); // ny må ligge sist, fordi ny kan ikke slettes, og dersom siste element slettes er selectedValue i dropdownlisten større en siste indeks
    functionList.setOptions(options);
}

void Animation::chooseFunction(){
    if (functionList.getSelectedValue() != currentFunction){
        t = 0;
        currentFunction = functionList.getSelectedValue();
        if (currentFunction == "Ny"){
            guitarString.coefficients = function.none();
        }
        else {
            guitarString.coefficients = function.getCoef(currentFunction);
        }
    }
}

void Animation::deleteFunction(){
    functionList.setVisible(false);
    next_frame();
    while (!should_close() && !is_key_down(KeyboardKey::ENTER)){
        draw_text({250, 250}, "Trykk enter for å slette funksjonen, eller tab for å avbryte", Color::black, 20);
        if (is_key_down(KeyboardKey::TAB)){
            functionList.setVisible(true);
            return;}
        next_frame();
    }    
    if (currentFunction != "Ny"){function.removeFunction(currentFunction);}
    guitarString.coefficients = function.none(); 
    currentFunction = "Ny"; // å slette funksjoner, inkludert "Ny", setter grafen til "Ny" og nullstiller
    updateOptions();
    functionList.setVisible(true);
}
