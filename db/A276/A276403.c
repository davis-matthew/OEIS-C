/*
 * A276403  a(n) = if n mod 6 = 0 then 4*3^((n-6)/3) elif n mod 6 = 1 then
 *          2^4*3^((n-10)/3) elif n mod 6 = 2 then 2^3*3^((n-8)/3) elif n mod
 *          6 = 3 then 2^2*3^((n-6)/3) elif n mod 6 = 4 then 2*3^((n-4)/3)
 *          otherwise 3^((n-2)/3).
 * https://oeis.org/A276403
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-6) for n>13. Closed forms based on n mod 6: n%6==0:
 *            4*3^((n-6)/3) n%6==1: 16*3^((n-10)/3) n%6==2: 8*3^((n-8)/3)
 *            n%6==3: 4*3^((n-6)/3) n%6==4: 2*3^((n-4)/3) n%6==5: 3^((n-2)/3)
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 62, u64 n <= 123,
 *            u128 n <= 244, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* power of an integer base to an integer exponent using binary
   exponentiation; works for arbitrary precision seq_t as long as
   multiplication is defined. */
static seq_t
pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 8)   /* undefined for n<8; behaviour does not matter */
        return 0;

    idx_t r = n % 6;
    seq_t val;
    idx_t exp;

    switch (r) {
    case 0: /* n = 6k */
        exp = (n - 6) / 3;
        val = 4 * pow_seq(3, exp);
        break;
    case 1: /* n = 6k+1 */
        exp = (n - 10) / 3;
        val = 16 * pow_seq(3, exp);
        break;
    case 2: /* n = 6k+2 */
        exp = (n - 8) / 3;
        val = 8 * pow_seq(3, exp);
        break;
    case 3: /* n = 6k+3 */
        exp = (n - 6) / 3;
        val = 4 * pow_seq(3, exp);
        break;
    case 4: /* n = 6k+4 */
        exp = (n - 4) / 3;
        val = 2 * pow_seq(3, exp);
        break;
    case 5: /* n = 6k+5 */
        exp = (n - 2) / 3;
        val = pow_seq(3, exp);
        break;
    default:
        val = 0; /* unreachable */
        break;
    }

    return val;
}
