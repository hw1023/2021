package stu.javabean;

public class Student {
	private String id;
	private String name;
	private String major;
	private String dept;
	private int score;
	private String mark;
	
	public Student() {
		
	}
	public Student(String id,String name,String major,String dept,int score,String mark) {
		this.id=id;
		this.name=name;
		this.major=major;
		this.dept=dept;
		this.score=score;
		this.mark=mark;

	}
	public String getMark() {
		return mark;
	}
	public void setMark(String mark) {
		this.mark = mark;
	}
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public int getScore() {
		return score;
	}
	public void setScore(int score) {
		this.score = score;
	}
	public String getDept() {
		return dept;
	}
	public void setDept(String dept) {
		this.dept = dept;
	}
	public String getMajor() {
		return major;
	}
	public void setMajor(String major) {
		this.major = major;
	}
}
