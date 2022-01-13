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
			line = sc.readLine();//从第二行开始读数据
			while(line!=null) {
				String[] fld=line.split("\\s+");//识别多个空格
		        if(!fld[0].trim().equals("")){
		        	int score=Integer.parseInt(fld[4]);//强制类型转化，字符串转换int
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
		    		}//分支结构将分数score转化为ABCD等级mark
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
	public ArrayList<Student> querById(String id) {//模糊查询
		sel.clear();
		for(Student s:all) {
			if(s.getId().trim().indexOf(id)!=-1) {
				sel.add(s);
			}
		}
		return sel;
	}
	public ArrayList<Student> equalToId(String id){//精准查询
		sel.clear();
		for(Student s:all) {
			if(s.getId().trim().equals(id)) {//查询内容是否匹配
				sel.add(s);
			}
		}
		return sel;//没有相匹配的内容则返回空数组sel
	}
	public ArrayList<Student> clearSel(String id){//返回空数组sel
		sel.clear();
		return sel;
	}
	public ArrayList<Student> orderByScore(){//按照成绩降序排名
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
				sw.write(s.getId()+" "+s.getName()+" "+s.getMajor()+" "+s.getMark()+"\n\r");//写入查询结果
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
