/*
 * A267168  Growth series for affine Coxeter group B_5.
 * https://oeis.org/A267168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((32*n^4+800*n^2+131)/189 - ((2*n^2+1) mod 3)*2/27 +
 *            (((n+5) mod 7) - ((n+1) mod 7) + ((4*n^4+2*n^2+1) mod 7))/7 +
 *            (((n+8) mod 9) + ((n+6) mod 9) - ((n+2) mod 9) - (n mod 9))/9) -
 *            [n=0]
 * Range:     u8 none, u16 n <= 5, u32 n <= 250, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n^2 and n^4 as seq_t to keep large values accurate */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;

    /* integer part of the main polynomial */
    seq_t A = 32 * n4 + 800 * n2 + 131;   /* all operations are exact */

    /* small mods of n */
    unsigned n_mod3 = n % 3;
    unsigned n_mod7 = n % 7;
    unsigned n_mod9 = n % 9;

    /* fractional contributions converted to a common denominator 189 */
    /* First fractional part  - ((2*n^2+1) mod 3)*2/27 */
    unsigned n2_mod3 = (n_mod3 * n_mod3) % 3;
    unsigned val1 = (2 * n2_mod3 + 1) % 3;               /* in {0,1,2} */
    long long c1 = - (long long)val1 * 2 * 7;           /* 189/27 = 7 */

    /* Second fractional part  (((n+5)%7-(n+1)%7+((4*n^4+2*n^2+1)%7))/7 */
    long long part =  (long long)((n_mod7 + 5) % 7) -
                      (long long)((n_mod7 + 1) % 7);

    unsigned n4_mod7 = (unsigned)(n4 % 7);
    unsigned n2_mod7 = (unsigned)(n2 % 7);
    unsigned temp = ( (4 * n4_mod7) % 7 +
                      (2 * n2_mod7) % 7 + 1 ) % 7;
    part += temp;
    long long c2 = part * 27;                      /* 189/7 = 27 */

    /* Third fractional part  (((n+8)%9 + (n+6)%9 - (n+2)%9 - n%9)/9) */
    long long part2 =
        (long long)((n_mod9 + 8) % 9) + (long long)((n_mod9 + 6) % 9) -
        (long long)((n_mod9 + 2) % 9) - (long long)(n_mod9);
    long long c3 = part2 * 21;                     /* 189/9 = 21 */

    /* Total adjustment numerator (can be negative) */
    long long C = c1 + c2 + c3;

    /* Add adjustment to the polynomial part */
    seq_t numerator;
    if (C >= 0) {
        numerator = A + (seq_t)C;
    } else {
        numerator = A - (seq_t)(-C);
    }

    /* integer division gives the floor of the fractional sum */
    seq_t val = numerator / 189;

    /* subtract the indicator [n==0] */
    if (n == 0)
        val -= 1;

    return val;
}
