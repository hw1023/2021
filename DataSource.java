package stu.util;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;

import javax.swing.JOptionPane;

public class DataSource {
	
	public static BufferedReader getDataReader(String path,String charset) {
		BufferedReader br=null;//局部变量赋初值
		try {
			br=new BufferedReader(new InputStreamReader(new FileInputStream(path),charset));
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			JOptionPane.showMessageDialog(null, "Charset not supported", "WOw", JOptionPane.INFORMATION_MESSAGE);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			JOptionPane.showMessageDialog(null, "File not found", "WOw", JOptionPane.INFORMATION_MESSAGE);
		}
		return br;
		
	}
	
	public static BufferedWriter setDataWriter(String path,String charset) {
		BufferedWriter sw=null;
		try {
			sw=new BufferedWriter (new OutputStreamWriter(new FileOutputStream(path),charset));
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return sw;
	}
	
}
