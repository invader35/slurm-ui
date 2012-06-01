import static org.junit.Assert.*;

import org.junit.Test;


public class PrimeTest {

	@Test
	public void test() {
		Prime deneme = new Prime();
		assertEquals("Result1",true,deneme.prime(3));
		assertEquals("Result2",true,deneme.prime(3));
		fail();
	}

}
