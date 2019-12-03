/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.mycompany.meteo;

import java.util.HashMap;

/**
 *
 * @author davide
 */
public class City {
    final public HashMap<String, Integer> temperature;
    
    public City(){
        temperature = new HashMap<>();
        temperature.put("roma", 21);
        temperature.put("napoli", 23);
        temperature.put("catania", 25);
        temperature.put("siracusa", 26);
        temperature.put("torino", 18);
    }
}
