/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.mycompany.newton;

import java.io.IOException;
import java.io.PrintWriter;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 *
 * @author davide
 */
@WebServlet(name = "Result", urlPatterns = {"/Result"})
public class Result extends HttpServlet {
    Double computeForce(Double f, Double m, Double a){
        if(f == 0.0)
            return m*a;
        else if(m == 0)
            return f/a;
        else
            return f/m;
    }
    protected void processRequest(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        response.setContentType("text/html;charset=UTF-8");
        try (PrintWriter out = response.getWriter()) {
            /* TODO output your page here. You may use following sample code. */
            out.println("<!DOCTYPE html>");
            out.println("<html>");
            out.println("<head>");
            out.println("<title>Compute Force Result</title>");            
            out.println("</head>");
            out.println("<body>");
            out.println("<h1>Computer Force Result</h1>");
            Double f, m, a;
            f = Double.parseDouble(request.getParameter("f"));
            m = Double.parseDouble(request.getParameter("m"));
            a = Double.parseDouble(request.getParameter("a"));
            
            if(f == 0.0 && m == 0.0 && a == 0.0)
                out.println("<b>Errore nell'input</b>");
            else if(f == 0.0)
                out.println("<b>F= " + computeForce(f, m, a).toString() + "</b>");
            else if(m == 0.0)
                out.println("<b>m= " + computeForce(f, m, a).toString() + "</b>");
            else
                out.println("<b>a= " + computeForce(f, m, a).toString() + "</b>");
                
            
            out.println("</body>");
            out.println("</html>");
        }
    }

    // <editor-fold defaultstate="collapsed" desc="HttpServlet methods. Click on the + sign on the left to edit the code.">
    /**
     * Handles the HTTP <code>GET</code> method.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        processRequest(request, response);
    }

    /**
     * Handles the HTTP <code>POST</code> method.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        processRequest(request, response);
    }

    /**
     * Returns a short description of the servlet.
     *
     * @return a String containing servlet description
     */
    @Override
    public String getServletInfo() {
        return "Short description";
    }// </editor-fold>

}
