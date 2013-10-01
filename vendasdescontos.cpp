#include<iostream>
#include<string>
#include<iomanip>
#include<cstdlib>
#include "include/JsonBox.h"


#define		CODIGO_DESCONTO_INEXISTENTE	2;
#define 	PRODUTO_INEXISTENTE		1;
#define		OK				0;



using namespace std;
using namespace JsonBox;




// Classe que representa o produto em si
class Product 

 {
     
 
   public:

    Product(unsigned id,string Name , double Price) 
         {
           ID = id;
           name = Name;
           price = Price;
         }


    void setNextProduct(Product * pointer)
      {
         nextProduct = pointer;
      }


    unsigned getID()
       {
         return ID;
       }

    string getName()         
       {
         return name;
       }
        
    float getPrice()
       {
         return price;
       }
   

   Product * getNextProduct()
      {
        return nextProduct;
      }

  
 
                       
   private : 
    unsigned ID;     
    string name;
    double price;
    Product* nextProduct;

 };




// Classe que representa o carrinho de compras
class Shoppingcart
  {
                
  
   public:

  //  Setters e Getters

    double getDiscount()
         {
            return discount;
         }



  // Metodo que gera calcula os descontos sejam eles percentuais ou fixos
     unsigned setDiscount(string couponCode , Array & ar2)
   
        {

           unsigned i = 0;
           bool foundDiscount = false;
        

            while(i<ar2.size())
               {
                     if(couponCode == ar2[i]["code"].getString())
                       {
                          foundDiscount = true;

                              if(ar2[i]["type"].getString() == "fixed")
                               {       
                                   fixedDiscount = true;
                                   discount = ar2[i]["amount"].getDouble();
                               }
                               else
                               {
                                   fixedDiscount = false;
                                   discount = ar2[i]["amount"].getDouble();
                               } 
                       }
                i++;

               }


            
           if(!foundDiscount)
            {
             cout<<"O desconto nao foi encontrado "<<endl;
             return CODIGO_DESCONTO_INEXISTENTE;
            }
 
 
           return OK;



        }






    
            
   // Metodo que adiciona um objeto produto ao objeto da classe Shoppingcart que nada mais e do que uma lista encadeada
      unsigned adicionarAoCarrinho(unsigned productID,Array & ar)
        {
           
           bool productExists = false;
           unsigned short i=0;
           unsigned teste;
           
           // parte do codigo que verifica se o codigo do produto representa um produto existente e que faz adicao 

             while(i<ar.size() && !productExists)
               {

                   
                     if(ar[i]["id"].getInt() == productID && firstProduct == 0)
                      {
                       currentProduct = new Product(ar[i]["id"].getInt(),ar[i]["name"].getString(),ar[i]["price"].getDouble());
                       ancientProduct = currentProduct;
                       firstProduct = currentProduct;
                       currentProduct->setNextProduct(NULL);
                       productExists = true;
                      }

                     else if(ar[i]["id"].getInt() == productID && firstProduct != 0)
                      {
                         currentProduct = new Product(ar[i]["id"].getInt(),ar[i]["name"].getString(),ar[i]["price"].getDouble());
                         ancientProduct->setNextProduct(currentProduct);
                         ancientProduct = currentProduct;
                         currentProduct->setNextProduct(NULL);
                         productExists = true;
                      }
                         
                          
                   i++;     
   

 
               }

    

            if(!productExists)
                {
                    cout<<"O produto nao existe "<<endl;
                    return PRODUTO_INEXISTENTE;
                }
             


            return OK;

        }





 // Metodo que verifica se a entrada productID eh um inteiro sem sinal e caso nao seja exibe uma mensagem 

  bool validateProductID(string productID)
    {

       unsigned i=0;
       bool idValido = true;
 
 // Vamos de 1 em 1 caracter procurando por pontos virgulas e sinais de menos , se forem encontrados o formato do numero eh invalido
       while(i<productID.size() && idValido)
          {
                 if(productID[i] != '0' && productID[i] != '1' && productID[i] != '2' && productID[i] != '3' 
                    && productID[i] != '4' && productID[i] != '5' && productID[i] != '6' && productID[i] != '7'
                     && productID[i] != '8' && productID[i] != '9')
                        idValido = false;
            
           i++;
          }
              
       return idValido;
    }




 // Metodo que exibe na tela o resumo das compras feitas e o valor total e o desconto
     void  purchaseResume()

    {
        
      Product * sweeper = firstProduct;
      sum = 0;

   
      
            while (sweeper != NULL)
             {
        
              cout<<sweeper->getID()<<"  "<<sweeper->getName()<<"  "<<setprecision(2)<<fixed<<sweeper->getPrice()<<endl;
              cout<<endl;
              sum += sweeper->getPrice();
              sweeper = sweeper->getNextProduct();
             }
         


     cout<<endl;
     cout<<endl;

           if(fixedDiscount)
            {
             cout<<"Descontos:   -"<<setprecision(2)<<fixed<<discount<<endl;
             cout<<endl;
             sum -= discount;
             cout<<"Total:        "<<setprecision(2)<<fixed<<sum<<endl;
            }
          else
            {
            discount = (discount/100)*sum;
            cout<<"Descontosp:   -"<<setprecision(2)<<fixed<<discount<<endl;
            cout<<endl;
            sum -= discount;
            cout<<"Total:        "<<setprecision(2)<<fixed<<sum<<endl;
            }

 
                      

    }




      // Calcular valor total dos produtos do carrinho
   float getTotalPrice()
        {
          
           Product* sweeper = firstProduct;
           float sum=0;

           while(sweeper->getNextProduct() != NULL)
                  sum += sweeper->getPrice();

          return sum;           

        } 




          
         
   private:

    Product* firstProduct;
    Product* ancientProduct;
    Product* currentProduct;
    double discount;
    double sum;
    bool  fixedDiscount;              // Variavel booleana verdadeira quando o desconto eh fixo
      

         
  };  








// Funcao que faz a mesma coisa que atoi fazia em C
 unsigned Atoi (string productID)
  
     {

        unsigned i = 0;
        char * auxProductID = new char [productID.size() + 1];
        
        while(i<productID.size())
          {
            auxProductID[i] = productID[i];
            i++;
          }

        auxProductID[i] = '\0';    
        

        return (unsigned) atoi(auxProductID);      


     }





// Inicio da funcao principal     
     
int main ()
     {



         string resposta;
         string codigoCupom;
         string productID;
         bool desejoAdicionarProduto = true;
         bool questionResolved = false;
         bool firstProductAdded = false;
         Shoppingcart* shoppingCart = new Shoppingcart;
         Value v;
         Value v2;
         Array ar;
         Array ar2;

         v.loadFromFile("product.json");
         v2.loadFromFile("discounts.json");

         ar = Array(v.getArray());         
         ar2 = Array(v2.getArray()); 

     
       
  // Primeira parte do programa referente aa adicao de produtos ao carrinho
  while(desejoAdicionarProduto)
   {
                      
                   
                  

                   
     cout<<"Digite a ID do produto que deseja adicionar"<<endl;
     cin>>productID;
 // Toda a execucao de codigo referente aa adicao de um produto pelo ID somente tem sentido se o usuario tiver digitado corretamente
       if(shoppingCart->validateProductID(productID))
         {   
                     
   // Este if de baixo serve para validar se a entrada e um inteiro sem sinal , o codigo so seque naturalmente se validarID = true   
              cout<<"Esta certo disso (S/N) ?"<<endl;
              cin>>resposta;

              do
              {          
                            if(!(resposta == "S") && !(resposta == "s") && !(resposta == "n") && !(resposta == "N")) 
                            {
                              cout<<"Voce so pode responder com (s/S/n/N)"<<endl;
                              cout<<endl;
                              cout<<"Esta certo disso (S/N) ?"<<endl;
                              cin>>resposta;
                            }
  
               }
                                   
               while(!(resposta == "S") && !(resposta == "s") && !(resposta == "n") && !(resposta == "N"));

            if(resposta == "S" || resposta == "s")
             if(shoppingCart->adicionarAoCarrinho(Atoi(productID),ar) == 0)     
                 if(resposta != "N" && resposta != "n")
                  {
                            cout<<"Deseja adicionar mais algum produto (S/N) ?"<<endl;
                            cin>>resposta;
   
                  // Este looping ai de baixo trata a possibilidade do usuario na responder som um caracter s/S/n/N
                            do
                            {
                              if(resposta == "S" || resposta == "s");
                              else if(resposta == "N" || resposta == "n");
                              else 
                                  {
                                   cout<<endl;
                                   cout<<"Voce so pode responder com (s/S/n/N)"<<endl;
                                   cout<<endl;
                                   cout<<"Deseja adicionar mais algum produto (S/N) ?"<<endl;
                                   cin>>resposta;
                                  }
                            }
                           while(!(resposta == "S") && !(resposta == "s") && !(resposta == "n") && !(resposta == "N"));


                            do
                            {
                              if(resposta == "S" || resposta == "s")
                                  desejoAdicionarProduto = true;
                              else if(resposta == "N" || resposta == "n")
                                  desejoAdicionarProduto = false;
                              else 
                                  {
                                   cout<<endl;
                                   cout<<"Voce so pode responder com (s/S/n/N)"<<endl;
                                   cout<<endl;
                                   cout<<"Deseja adicionar mais algum produto (S/N) ?"<<endl;
                                   cin>>resposta;
                                  }
                            }
                           while(!(resposta == "S") && !(resposta == "s") && !(resposta == "n") && !(resposta == "N"));

                          



                 }

         }
           
        else 
           {
           cout<<endl;
           cout<<"Por favor, digite uma ID que tenha algum sentido "<<endl; 
           }
       cout<<endl;                   
   }  
               


   // Segunda parte do programa referente aa obtencao de um desconto
      do
       {

 
         cout<<"Deseja adicionar um cupom de desconto (S/N) ?"<<endl;
         cin>>resposta;
    
                 if(resposta == "S" || resposta == "s")
                 {
                     do
                      {
                      cout<<endl;
                      cout<<"Esta certo disso (S/N) ?"<<endl;
                      cin>>resposta;
                         if(resposta == "S" || resposta == "s")
                           {
                             cout<<endl;
                             cout<<"Digite o codigo do cupom"<<endl;
                             cin>>codigoCupom;
                       
                                  if(shoppingCart->setDiscount(codigoCupom,ar2))
                                     {
                                       cout<<endl;
                                       cout<<"Nao foi encontrado nenhum cupom com este codigo "<<endl;
                                      
                                     }
                                  else
                                      questionResolved = true;
                                    

                           }
                           else if(resposta == "N" || resposta == "n");

                           else
                             {
                             cout<<endl; 
                             cout<<"Voce so pode responder com (s/S/n/N)"<<endl;
                        
                             }

                       }
                      while(resposta != "S" && resposta != "s" && resposta != "N" && resposta != "n");

                  }


                  else if (resposta == "N" || resposta == "n")
                     questionResolved = true;
                      


                  else
                     {
                     cout<<endl; 
                     cout<<"Voce so pode responder com (s/S/n/N) 2"<<endl;
                     resposta = "S";
                     }

         cout<<endl;
            


         
         cout<<endl;
     }
     while(!questionResolved);


  
  
  

  // Parte do codigo que diz qual eh o desconto e exibe o resumo das compras       
   shoppingCart->purchaseResume();
   

   return OK;   
            
     }         






