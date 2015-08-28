
import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

import org.junit.*;

import static org.junit.Assert.*;

/**
 * JUnit tests for CS 25100 Spring 2015 Project 4.
 * 
 * @author Neil Allison
 * @version 04/24/2015
 */
public class PolynomialTest {
	private Polynomial p;
	private ByteArrayOutputStream baos;

	private String[] testExpressions = {
			"3",					// 0
			"2+y",
			"-3",
			"5-3",
			"5-x",
			"5-6*x",				// 5
			"5-x^7",
			"5-(-x-3)^7",
			"4-5*x^2",
			"a-(b-c)",
			"a+b*c+d",				// 10
			"(a^2)*3-c+5",
			"-(-5*x)^3",
			"(x+y)*(x+5)*(2x-yz)",
			"(x^2-y^3)",
			"(a+b-c)^2",			// 15
			"44x-(a-(x^2-y^3))",
			"(abc+11)^4",
			"(a+b)*(c+d)",
			"x^2-y^32",
			"(x+y)^2",				// 20
			"(x+y)*(x-y)",
			"(x+y)^2-(x-y)^2",
			"(a+b-c)^2",
			"a-(x^2-(k+6n)^3-(z^2-j^3))",
			"(x+z)^2-(x+y)^2",				// 25
			"a-(b+c*(d-(e+f-g)))",
			"(2*x+y)*(2*x-y)",
			"a-(b-((x+y)^2-(44-55)^5))",
			"a-(x^2-y^3-(z^8-j^9))",
			"a-(b-((x+y)^2-(55-44)^5))",	// 30
			"6*7*(a+h)*(1-1)", 
			"6*(6-5)",
			"(2*x-3*y)*(2*x+3*y)",
			"(2x-3y)*(x+3y)",
			"(abc+11)^4", 					// 35
			"(a-b)*(a^2+ab+b^2)",
			"(a+b)*(a^2-ab+b^2)", 
			"3*a*6*(a+a+4*a)*b*a",
			"a^2-(bc)^3",
			"(x+y)*(x+5)*(2*x-yz)",			// 40
			"(1-x)*(1+x)*(1+x^2)*(1+x^4)*(1+x^8)", 
			"(1+x)^9",
			"-x^2+(-2x+1)*(-2x-1)",
			"3+(-x^2)+(-2)+(-(-2x^2))",
			"((1+x)^9+2)^0",				// 45
			"1999+(1-a)*(1+a)*(1+a^2)*(1+a^4)*(1+a^8)*(1+a^16)+2001a^32"
	};

	private String[] expectedResults = {
			"[3]",																	// 0
			"[+, [2], [y]]",
			"[-3]",
			"[+, [5], [-3]]",
			"[+, [5], [-x]]",
			"[+, [5], [*, [-6], [x]]]",												// 5
			"[+, [5], -[^, [x], [7]]]",
			"[+, [5], -[^, [+, [-x], [-3]], [7]]]",
			"[+, [4], [*, [-5], [^, [x], [2]]]]",
			"[+, [a], [+, [-b], [c]]]",
			"[+, [a], [*, [b], [c]], [d]]",											// 10
			"[+, [*, [^, [a], [2]], [3]], [-c], [5]]",
			"-[^, [*, [-5], [x]], [3]]",
			"[*, [+, [x], [y]], [+, [x], [5]], [+, [*, [2], [x]], [-yz]]]",
			"[+, [^, [x], [2]], -[^, [y], [3]]]",
			"[^, [+, [a], [b], [-c]], [2]]",										// 15
			"[+, [*, [44], [x]], [+, [-a], [+, [^, [x], [2]], -[^, [y], [3]]]]]",
			"[^, [+, [abc], [11]], [4]]",
			"[*, [+, [a], [b]], [+, [c], [d]]]",
			"[+, [^, [x], [2]], -[^, [y], [32]]]",
			"[^, [+, [x], [y]], [2]]",												// 20
			"[*, [+, [x], [y]], [+, [x], [-y]]]",
			"[+, [^, [+, [x], [y]], [2]], -[^, [+, [x], [-y]], [2]]]",
			"[^, [+, [a], [b], [-c]], [2]]",
			"[+, [a], [+, -[^, [x], [2]], [^, [+, [k], [*, [6], [n]]], [3]], [+, [^, [z], [2]], -[^, [j], [3]]]]]",
			"[+, [^, [+, [x], [z]], [2]], -[^, [+, [x], [y]], [2]]]",				// 25
			"[+, [a], [+, [-b], [*, [-c], [+, [d], [+, [-e], [-f], [g]]]]]]",
			"[*, [+, [*, [2], [x]], [y]], [+, [*, [2], [x]], [-y]]]",
			"[+, [a], [+, [-b], [+, [^, [+, [x], [y]], [2]], -[^, [+, [44], [-55]], [5]]]]]",
			"[+, [a], [+, -[^, [x], [2]], [^, [y], [3]], [+, [^, [z], [8]], -[^, [j], [9]]]]]",
			"[+, [a], [+, [-b], [+, [^, [+, [x], [y]], [2]], -[^, [+, [55], [-44]], [5]]]]]",		// 30
			"[*, [6], [7], [+, [a], [h]], [+, [1], [-1]]]",
			"[*, [6], [+, [6], [-5]]]",
			"[*, [+, [*, [2], [x]], [*, [-3], [y]]], [+, [*, [2], [x]], [*, [3], [y]]]]",
			"[*, [+, [*, [2], [x]], [*, [-3], [y]]], [+, [x], [*, [3], [y]]]]",
			"[^, [+, [abc], [11]], [4]]",															// 35
			"[*, [+, [a], [-b]], [+, [^, [a], [2]], [ab], [^, [b], [2]]]]",
			"[*, [+, [a], [b]], [+, [^, [a], [2]], [-ab], [^, [b], [2]]]]",
			"[*, [3], [a], [6], [+, [a], [a], [*, [4], [a]]], [b], [a]]",
			"[+, [^, [a], [2]], -[^, [bc], [3]]]",
			"[*, [+, [x], [y]], [+, [x], [5]], [+, [*, [2], [x]], [-yz]]]",							// 40
			"[*, [+, [1], [-x]], [+, [1], [x]], [+, [1], [^, [x], [2]]], [+, [1], [^, [x], [4]]], [+, [1], [^, [x], [8]]]]",
			"[^, [+, [1], [x]], [9]]",
			"[+, -[^, [x], [2]], [*, [+, [*, [-2], [x]], [1]], [+, [*, [-2], [x]], [-1]]]]",
			"[+, [3], -[^, [x], [2]], [-2], [*, [2], [^, [x], [2]]]]",
			"[^, [+, [^, [+, [1], [x]], [9]], [2]], [0]]",											// 45
			"[+, [1999], [*, [+, [1], [-a]], [+, [1], [a]], [+, [1], [^, [a], [2]]], [+, [1], [^, [a], [4]]], [+, [1], [^, [a], [8]]], [+, [1], [^, [a], [16]]]], [*, [2001], [^, [a], [32]]]]"
	};
	
	private String[] expectedFinalExpressions = {
			"3",				// 0
			"2 + y",
			"- 3",
			"2",
			"5 - x",
			"5 - 6x",			// 5
			"5 - x^7",
			"x^7 + 21x^6 + 189x^5 + 945x^4 + 2835x^3 + 5103x^2 + 5103x + 2192",
			"4 - 5x^2",
			"a - b + c",
			"a + bc + d",					// 10
			"3a^2 - c + 5",
			"125x^3",
			"2x^3 - x^2yz + 2x^2y + 10x^2 - xy^2z - 5xyz + 10xy - 5y^2",
			"x^2 - y^3",
			"a^2 + 2ab - 2ac + b^2 - 2bc + c^2",					// 15
			"",
			"",
			"",
			"- y^32 + x^2",
			"x^2 + 2xy + y^2",					// 20
			"x^2 - y^2",
			"4xy",
			"a^2 + 2ab - 2ac + b^2 - 2bc + c^2",
			"",
			"",					// 25
			"",
			"",
			"",
			"",
			"",					// 30
			"",
			"",
			"",
			"",
			"",					// 35
			"",
			"",
			"",
			"",
			"",					// 40
			"",
			"",
			"",
			"",
			"",					// 45
			""
	};

	@Before
	public void setUp() {
		baos = new ByteArrayOutputStream();
		System.setOut(new PrintStream(baos));
	}

	@After
	public void cleanUpStream() {
		System.setOut(null);
	}

	@Test(timeout = 100)
	public void testConvertToListRepresentation0() {
		p = new Polynomial(testExpressions[0]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[0];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[0];

		assertEquals(message, expectedOut, actualOut);
	}

	@Test(timeout = 100)
	public void testConvertToListRepresentation1() {
		p = new Polynomial(testExpressions[1]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[1];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[1];

		assertEquals(message, expectedOut, actualOut);
	}

	@Test(timeout = 100)
	public void testConvertToListRepresentation2() {
		p = new Polynomial(testExpressions[2]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[2];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[2];

		assertEquals(message, expectedOut, actualOut);
	}

	@Test(timeout = 100)
	public void testConvertToListRepresentation3() {
		p = new Polynomial(testExpressions[3]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[3];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[3];

		assertEquals(message, expectedOut, actualOut);
	}

	@Test(timeout = 100)
	public void testConvertToListRepresentation4() {
		p = new Polynomial(testExpressions[4]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[4];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[4];

		assertEquals(message, expectedOut, actualOut);
	}

	@Test(timeout = 100)
	public void testConvertToListRepresentation5() {
		p = new Polynomial(testExpressions[5]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[5];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[5];

		assertEquals(message, expectedOut, actualOut);
	}

	@Test(timeout = 100)
	public void testConvertToListRepresentation6() {
		p = new Polynomial(testExpressions[6]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[6];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[6];

		assertEquals(message, expectedOut, actualOut);
	}

	@Test(timeout = 100)
	public void testConvertToListRepresentation7() {
		p = new Polynomial(testExpressions[7]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[7];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[7];

		assertEquals(message, expectedOut, actualOut);
	}

	@Test(timeout = 100)
	public void testConvertToListRepresentation8() {
		p = new Polynomial(testExpressions[8]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[8];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[8];

		assertEquals(message, expectedOut, actualOut);
	}

	@Test(timeout = 100)
	public void testConvertToListRepresentation9() {
		p = new Polynomial(testExpressions[9]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[9];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[9];

		String alternateExpectedOut = "[+, [a], [-b], [c]]";
		
		if (actualOut.equals(alternateExpectedOut)) {
			assertEquals(message, alternateExpectedOut, actualOut);
		} else {
			assertEquals(message, expectedOut, actualOut);
		}
	}

	@Test(timeout = 100)
	public void testConvertToListRepresentation10() {
		p = new Polynomial(testExpressions[10]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[10];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[10];

		assertEquals(message, expectedOut, actualOut);
	}

	@Test(timeout = 100)
	public void testConvertToListRepresentation11() {
		p = new Polynomial(testExpressions[11]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[11];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[11];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation12() {
		p = new Polynomial(testExpressions[12]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[12];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[12];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation13() {
		p = new Polynomial(testExpressions[13]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[13];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[13];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation14() {
		p = new Polynomial(testExpressions[14]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[14];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[14];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation15() {
		p = new Polynomial(testExpressions[15]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[15];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[15];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation16() {
		p = new Polynomial(testExpressions[16]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[16];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[16];

		String alternateExpectedOut = "[+, [*, [44], [x]], [-a], [^, [x], [2]], -[^, [y], [3]]]";
		
		if (actualOut.equals(alternateExpectedOut)) {
			assertEquals(message, alternateExpectedOut, actualOut);
		} else {
			assertEquals(message, expectedOut, actualOut);
		}
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation17() {
		p = new Polynomial(testExpressions[17]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[17];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[17];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation18() {
		p = new Polynomial(testExpressions[18]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[18];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[18];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation19() {
		p = new Polynomial(testExpressions[19]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[19];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[19];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation20() {
		p = new Polynomial(testExpressions[20]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[20];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[20];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation21() {
		p = new Polynomial(testExpressions[21]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[21];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[21];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation22() {
		p = new Polynomial(testExpressions[22]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[22];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[22];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation23() {
		p = new Polynomial(testExpressions[23]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[23];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[23];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation24() {
		p = new Polynomial(testExpressions[24]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[24];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[24];

		String alternateExpectedOut = "[+, [a], -[^, [x], [2]], [^, [+, [k], [*, [6], [n]]], [3]], [^, [z], [2]], -[^, [j], [3]]]";
		
		if (actualOut.equals(alternateExpectedOut)) {
			assertEquals(message, alternateExpectedOut, actualOut);
		} else {
			assertEquals(message, expectedOut, actualOut);
		}
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation25() {
		p = new Polynomial(testExpressions[25]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[25];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[25];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation26() {
		p = new Polynomial(testExpressions[26]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[26];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[26];

		String alternateExpectedOut = "[+, [a], [-b], [*, [-c], [+, [d], [-e], [-f], [g]]]]";
		
		if (actualOut.equals(alternateExpectedOut)) {
			assertEquals(message, alternateExpectedOut, actualOut);
		} else {
			assertEquals(message, expectedOut, actualOut);
		}
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation27() {
		p = new Polynomial(testExpressions[27]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[27];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[27];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation28() {
		p = new Polynomial(testExpressions[28]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[28];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[28];

		String alternateExpectedOut = "[+, [a], [-b], [^, [+, [x], [y]], [2]], -[^, [+, [44], [-55]], [5]]]";
		
		if (actualOut.equals(alternateExpectedOut)) {
			assertEquals(message, alternateExpectedOut, actualOut);
		} else {
			assertEquals(message, expectedOut, actualOut);
		}
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation29() {
		p = new Polynomial(testExpressions[29]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[29];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[29];

		String alternateExpectedOut = "[+, [a], -[^, [x], [2]], [^, [y], [3]], [^, [z], [8]], -[^, [j], [9]]]";
		
		if (actualOut.equals(alternateExpectedOut)) {
			assertEquals(message, alternateExpectedOut, actualOut);
		} else {
			assertEquals(message, expectedOut, actualOut);
		}
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation30() {
		p = new Polynomial(testExpressions[30]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[30];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[30];

		String alternateExpectedOut = "[+, [a], [-b], [^, [+, [x], [y]], [2]], -[^, [+, [55], [-44]], [5]]]";
		
		if (actualOut.equals(alternateExpectedOut)) {
			assertEquals(message, alternateExpectedOut, actualOut);
		} else {
			assertEquals(message, expectedOut, actualOut);
		}
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation31() {
		p = new Polynomial(testExpressions[31]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[31];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[31];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation32() {
		p = new Polynomial(testExpressions[32]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[32];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[32];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation33() {
		p = new Polynomial(testExpressions[33]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[33];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[33];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation34() {
		p = new Polynomial(testExpressions[34]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[34];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[34];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation35() {
		p = new Polynomial(testExpressions[35]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[35];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[35];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation36() {
		p = new Polynomial(testExpressions[36]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[36];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[36];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation37() {
		p = new Polynomial(testExpressions[37]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[37];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[37];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation38() {
		p = new Polynomial(testExpressions[38]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[38];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[38];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation39() {
		p = new Polynomial(testExpressions[39]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[39];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[39];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation40() {
		p = new Polynomial(testExpressions[40]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[40];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[40];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation41() {
		p = new Polynomial(testExpressions[41]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[41];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[41];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation42() {
		p = new Polynomial(testExpressions[42]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[42];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[42];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation43() {
		p = new Polynomial(testExpressions[43]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[43];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[43];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation44() {
		p = new Polynomial(testExpressions[44]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[44];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[44];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation45() {
		p = new Polynomial(testExpressions[45]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[45];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[45];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testConvertToListRepresentation46() {
		p = new Polynomial(testExpressions[46]);
		Utils.printListRepresentation(p.listRepresentation);

		String expectedOut = expectedResults[46];
		String actualOut = baos.toString();
		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[46];

		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression0() {
		p = new Polynomial(testExpressions[0]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[0] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[0];
		
		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression1() {
		p = new Polynomial(testExpressions[1]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[1] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[1];
		
		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression2() {
		p = new Polynomial(testExpressions[2]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[2] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[2];
		
		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression3() {
		p = new Polynomial(testExpressions[3]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[3] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[3];
		
		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression4() {
		p = new Polynomial(testExpressions[4]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[4] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[4];
		
		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression5() {
		p = new Polynomial(testExpressions[5]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[5] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[5];
		
		String alternateExpectedOut = "- 6x + 5" + newline;
		
		if (actualOut.equals(alternateExpectedOut)) {
			assertEquals(message, alternateExpectedOut, actualOut);
		} else {
			assertEquals(message, expectedOut, actualOut);
		}
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression6() {
		p = new Polynomial(testExpressions[6]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[6] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[6];
		
		String alternateExpectedOut = "- x^7 + 5" + newline;
		
		if (actualOut.equals(alternateExpectedOut)) {
			assertEquals(message, alternateExpectedOut, actualOut);
		} else {
			assertEquals(message, expectedOut, actualOut);
		}
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression7() {
		p = new Polynomial(testExpressions[7]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[7] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[7];
		
		assertEquals(message, expectedOut, actualOut);
	}
	/*
	@Test(timeout = 100)
	public void testEvaluateExpression8() {
		p = new Polynomial(testExpressions[8]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[8] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[8];
		
		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression9() {
		p = new Polynomial(testExpressions[9]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[9] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[9];
		
		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression10() {
		p = new Polynomial(testExpressions[10]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[10] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[10];
		
		String alternateExpectedOut = "bc + a + d" + newline;
		
		if (actualOut.equals(alternateExpectedOut)) {
			assertEquals(message, alternateExpectedOut, actualOut);
		} else {
			assertEquals(message, expectedOut, actualOut);
		}
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression11() {
		p = new Polynomial(testExpressions[11]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[11] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[11];
		
		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression12() {
		p = new Polynomial(testExpressions[12]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[12] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[12];
		
		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression13() {
		p = new Polynomial(testExpressions[13]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[13] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[13];
		
		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression14() {
		p = new Polynomial(testExpressions[14]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[14] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[14];
		
		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression15() {
		p = new Polynomial(testExpressions[15]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[15] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[15];
		
		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression19() {
		p = new Polynomial(testExpressions[19]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[19] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[19];
		
		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression20() {
		p = new Polynomial(testExpressions[20]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[20] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[20];
		
		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression21() {
		p = new Polynomial(testExpressions[21]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[21] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[21];
		
		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression22() {
		p = new Polynomial(testExpressions[22]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[22] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[22];
		
		assertEquals(message, expectedOut, actualOut);
	}
	
	@Test(timeout = 100)
	public void testEvaluateExpression23() {
		p = new Polynomial(testExpressions[23]);
		p.evaluate();
		Utils.printOutputExpression(p.finalExpression);
		
		String newline = System.getProperty("line.separator");
		String expectedOut = expectedFinalExpressions[23] + newline;
		String actualOut = baos.toString();
		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[23];
		
		assertEquals(message, expectedOut, actualOut);
	}
	*/
//	@Test(timeout = 100)
//	public void testEvaluateExpression#() {
//		p = new Polynomial(testExpressions[#]);
//		p.evaluate();
//		Utils.printOutputExpression(p.finalExpression);
//		
//		String newline = System.getProperty("line.separator");
//		String expectedOut = expectedFinalExpressions[#] + newline;
//		String actualOut = baos.toString();
//		String message = "evaluateExpression(): test failed for the following polynomial: " + testExpressions[#];
//		
//		assertEquals(message, expectedOut, actualOut);
//	}
	
//	@Test(timeout = 100)
//	public void testConvertToListRepresentation#() {
//		p = new Polynomial(testExpressions[#]);
//		Utils.printListRepresentation(p.listRepresentation);
//
//		String expectedOut = expectedResults[#];
//		String actualOut = baos.toString();
//		String message = "convertToListRepresentation(): test failed for the following polynomial: " + testExpressions[#];
//
//		assertEquals(message, expectedOut, actualOut);
//	}
}
