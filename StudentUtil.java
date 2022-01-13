package stu.util;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;

import stu.javabean.Student;

public class StudentUtil {
	ArrayList<Student> all=new ArrayList<Student>();
	ArrayList<Student> sel=new ArrayList<Student>();
	public StudentUtil(BufferedReader sc) {
		String line;
		try {
			line = sc.readLine();
			line = sc.readLine();//�ӵڶ��п�ʼ������
			while(line!=null) {
				String[] fld=line.split("\\s+");//ʶ�����ո�
		        if(!fld[0].trim().equals("")){
		        	int score=Integer.parseInt(fld[4]);//ǿ������ת�����ַ���ת��int
		        	String mark=null;
		    		if(60>score) {
		    			mark="D";
		    		}else if(75>score) {
		    			mark="C";
		    		}else if(90>score) {
		    			mark="B";
		    		}else if(100>=score) {
		    			mark="A";
		    		}else {
		    			mark="error";
		    		}//��֧�ṹ������scoreת��ΪABCD�ȼ�mark
		        	Student stu=new Student(fld[0],fld[1],fld[2],fld[3],score,mark);// Integer.toString()
		            all.add(stu);
		          }
		        line=sc.readLine();
		           
		     }//end of while
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	    
	}
	public ArrayList<Student> queryAll() {
		sel.clear();
		for(Student s:all) {
			sel.add(s);
		}
		return sel;
	}
	public ArrayList<Student> querById(String id) {//ģ����ѯ
		sel.clear();
		for(Student s:all) {
			if(s.getId().trim().indexOf(id)!=-1) {
				sel.add(s);
			}
		}
		return sel;
	}
	public ArrayList<Student> equalToId(String id){//��׼��ѯ
		sel.clear();
		for(Student s:all) {
			if(s.getId().trim().equals(id)) {//��ѯ�����Ƿ�ƥ��
				sel.add(s);
			}
		}
		return sel;//û����ƥ��������򷵻ؿ�����sel
	}
	public ArrayList<Student> clearSel(String id){//���ؿ�����sel
		sel.clear();
		return sel;
	}
	public ArrayList<Student> orderByScore(){//���ճɼ���������
		sel.sort(new Comparator<Student>() {

			@Override
			public int compare(Student arg0, Student arg1) {
				// TODO Auto-generated method stub
				return arg1.getScore()-arg0.getScore();
			}
			
		});
		return sel;
	}

	//writer result to file
	public void writerData(ArrayList<Student> result,BufferedWriter sw) {
		 for(Student s:result){
		     try {
				sw.write(s.getId()+" "+s.getName()+" "+s.getMajor()+" "+s.getMark()+"\n\r");//д���ѯ���
			    sw.newLine();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		 }//end of for
	}
	//closewriter
	public void closeBw(BufferedWriter sw) {
		if(sw!=null) {
			try {
				sw.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	public void closeBr(BufferedReader br) {
		if(br!=null) {
			try {
				br.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}//end of if
	}//end of method
	
}
