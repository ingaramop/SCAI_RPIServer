import java.awt.Image;
import java.awt.image.BufferedImage;
import java.awt.image.RenderedImage;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import javax.imageio.ImageIO;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;


public class Main {
	//timing variables
	private static final String USER_AGENT = "Mozilla/5.0";
	private static final int HTTP_READ_TIMEOUT = 3000; //milliseconds
    private static final int HTTP_CONNECT_TIMEOUT = 3000; //  milliseconds
    private static final int DATABASE_UPDATE_INTERVAL = 1;// minutes
    //addresses
    private static final String VIDEO_ADDRESS_DEFAULT = "http://192.168.1.10/videostream.cgi?user=pi&pwd=raspberry&resolution=32&rate=2";
    private static final String IMU_QUERY_ADDRESS_DEFAULT = "http://127.0.0.1/cgi-bin/imuQueryMock.fcgi";
    private static final String GPS_QUERY_ADDRESS_DEFAULT = "http://127.0.0.1/cgi-bin/gpsQueryMock.fcgi";
    //parsing constants IMU
    private static final String TIPPER_INCLINATION = "tipperInclination";
    private static final String SIDE_INCLINATION = "sideInclination";
    private static final String COMPASS = "compass";
    private static final String TEMPERATURE = "temperature";
    private static final String PRESSURE = "pressure";
    //parsing constants GPS
    private static final String POSITION_X = "positionX";
    private static final String POSITION_Y = "positionY";
    private static final String TIMESTAMP = "timestamp";
    //db credentials
    private static final String MYSQL_USER = "pi";
    private static final String MYSQL_PASS = "raspberry";
 // JDBC driver name and database URL
    private static final String JDBC_DRIVER = "com.mysql.jdbc.Driver";  
    private static final String DB_URL = "jdbc:mysql://localhost/EMP";

    
    //non final variables
    private static Element eElement;
    
    private static String tipperInclination;
    private static String sideInclination;
    private static String compass;
    private static String temperature;
    private static String pressure;
    private static String positionX;
    private static String positionY;
    private static String timestamp;
    
    
    
	
	
	public static void main(String[] args) {
		InputStream stream;
		
		// repeat idefinitely
		while(true){	
			
			//get IMU web service data
			eElement = HTTPGetAndParse(IMU_QUERY_ADDRESS_DEFAULT);
			if(eElement == null){
				tipperInclination = "";
				sideInclination = "";
				compass = "";
				temperature = "";
				pressure = "";
			}
			else{
				try{
					tipperInclination = eElement.getElementsByTagName(TIPPER_INCLINATION).item(0).getTextContent();
					sideInclination = eElement.getElementsByTagName(SIDE_INCLINATION).item(0).getTextContent();
					compass = eElement.getElementsByTagName(COMPASS).item(0).getTextContent();
					temperature = eElement.getElementsByTagName(TEMPERATURE).item(0).getTextContent();
					pressure = eElement.getElementsByTagName(PRESSURE).item(0).getTextContent();
				}catch (Exception e){
					e.printStackTrace();
					tipperInclination = "";
					sideInclination = "";
					compass = "";
					temperature = "";
					pressure = "";
				}			
			}
	
			//get GPS web service data
			eElement = HTTPGetAndParse(GPS_QUERY_ADDRESS_DEFAULT);
			if(eElement == null){
					positionX = "";
					positionY = "";
					timestamp = "";				
			}
			else{
				try{
					positionX = eElement.getElementsByTagName(POSITION_X).item(0).getTextContent();
					positionY = eElement.getElementsByTagName(POSITION_Y).item(0).getTextContent();
					timestamp = eElement.getElementsByTagName(TIMESTAMP).item(0).getTextContent();
				}
				catch(Exception e){
					e.printStackTrace();
					positionX = "";
					positionY = "";
					timestamp = "";		
				}
			}
			
			System.out.println(tipperInclination);
			System.out.println(sideInclination);
			System.out.println(compass);
			System.out.println(temperature);
			System.out.println(pressure);
			System.out.println(positionX);
			System.out.println(positionY);
			System.out.println(timestamp);
			System.out.println();
			
		
			//get snapshot from camera
			URL imageURL;
			try {
				imageURL = new URL("http://192.168.1.2/snapshot.cgi?user=pi&pwd=raspberry");
				// Case 1
			    BufferedImage img = ImageIO.read(imageURL);
			    System.out.println(img);
			} catch (MalformedURLException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
					    
			//store obtained data into database
			dbStoreValues();		
				
			// Sleep for DATABASE_UPDATE_INTERVAL minutes
			try {
				Thread.sleep(DATABASE_UPDATE_INTERVAL *   // minutes to sleep
				         60 *   // seconds to a minute
				         1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} // milliseconds to a second
		}
	}
		
		
	private static void dbStoreValues(){
		Connection conn = null;
		   Statement stmt = null;
		   try{
		      //STEP 2: Register JDBC driver
		      Class.forName(JDBC_DRIVER);

		      //STEP 3: Open a connection
		      System.out.println("Connecting to database...");
		      conn = DriverManager.getConnection(DB_URL,MYSQL_USER,MYSQL_PASS);

		      //STEP 4: Execute a query
		      System.out.println("Creating statement...");
		      stmt = conn.createStatement();
		      String sql;
		      sql = "SELECT id, first, last, age FROM Employees";
		      ResultSet rs = stmt.executeQuery(sql);

		      //STEP 5: Extract data from result set
		      while(rs.next()){
		         //Retrieve by column name
		         int id  = rs.getInt("id");
		         int age = rs.getInt("age");
		         String first = rs.getString("first");
		         String last = rs.getString("last");

		         //Display values
		         System.out.print("ID: " + id);
		         System.out.print(", Age: " + age);
		         System.out.print(", First: " + first);
		         System.out.println(", Last: " + last);
		      }
		      //STEP 6: Clean-up environment
		      rs.close();
		      stmt.close();
		      conn.close();
		   }catch(SQLException se){
		      //Handle errors for JDBC
		      se.printStackTrace();
		   }catch(Exception e){
		      //Handle errors for Class.forName
		      e.printStackTrace();
		   }finally{
		      //finally block used to close resources
		      try{
		         if(stmt!=null)
		            stmt.close();
		      }catch(SQLException se2){
		      }// nothing we can do
		      try{
		         if(conn!=null)
		            conn.close();
		      }catch(SQLException se){
		         se.printStackTrace();
		      }//end finally try
		   }//end try
	}
	
	private static Element HTTPGetAndParse(String URL) {
        try {
        	///////HTTP GET////////
            URL url = new URL(URL);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setReadTimeout(HTTP_READ_TIMEOUT /* milliseconds */);
            conn.setConnectTimeout(HTTP_CONNECT_TIMEOUT /* milliseconds */);
            conn.setRequestMethod("GET");// optional default is GET
            conn.setRequestProperty("User-Agent", USER_AGENT);//add request header
            conn.setDoInput(true);
            conn.connect();

            //add request header
            // con.setRequestProperty("User-Agent", USER_AGENT);

            int responseCode = conn.getResponseCode();
            if (responseCode != 200) {
                conn.disconnect();
                return null;// if response is not success (200), then return null
            }

            InputStream stream = conn.getInputStream();
            
            
            ///////BUILD NODELIST////////
        	DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
        	DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
        	Document doc = dBuilder.parse(stream);
        			
        	//optional, but recommended
        	//read this - http://stackoverflow.com/questions/13786607/normalization-in-dom-parsing-with-java-how-does-it-work
        	doc.getDocumentElement().normalize();

        	//System.out.println("Root element :" + doc.getDocumentElement().getNodeName());
        			
        	NodeList nList = doc.getElementsByTagName("Response");
        	
        	Node nNode = nList.item(0);
        	Element eElement = (Element) nNode;
            return eElement;// else, return inputstream
            
            
        } catch (ProtocolException e) { //print stack trace and return null for all exceptions
            e.printStackTrace();
        } catch (MalformedURLException f) {
            f.printStackTrace();
        } catch (IOException g) {
            g.printStackTrace();
        } catch (Exception e) {
        	e.printStackTrace();
        }
        return null;
    }


}
