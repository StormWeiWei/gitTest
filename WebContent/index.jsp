<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
<script type="text/javascript" src="jquery-3.2.1.js"></script>
<script type="text/javascript">
	function recognize() {
		var formData = new FormData($("#myFaceForm")[0]);
				$.ajax({
					url : 'FaceServlet',
					type : 'POST',
					data : formData,
					async : true,
					contentType : false,
					processData : false,
					success : function(returnData) {
						var resultData = jQuery.parseJSON(returnData);
						if (resultData.result == null
								|| resultData.result.length == 0) {
							alert("您传的不是人脸");
						} else {
							var beautyScore = Math
									.round(resultData.result[0].beauty);
							var ageScore = Math.round(resultData.result[0].age);

							$("#beautyId").html(beautyScore);
							$("#ageId").html(ageScore);

						}
					},
					error : function(resultData) {
						alert(returnData);
					}
				});
	}
</script>
</head>
<body>
	<form id="myFaceForm" enctype="multipart/form-data">
		上传照片：<input type="file" name="myfile"><br> <input
			type="button" value="识别" onclick="recognize()">
	</form>
	
	<br> 根据百度AI，此图的人脸识别结果如下：
	<br> 颜值：
	<font id="beautyId" color="blue"></font>
	<br> 年龄：
	<font id="ageId" color="blue"></font>
</body>
</html>