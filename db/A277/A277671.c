/*
 * A277671  Number of n-length words over an 8-ary alphabet {a_1,a_2,...,a_8}
 *          avoiding consecutive letters a_i, a_{i+1}.
 * https://oeis.org/A277671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 7*a(n-2) + 6*a(n-3) - 5*a(n-4) + 4*a(n-5) -
 *            3*a(n-6) + 2*a(n-7) - a(n-8) (n ≥ 8) with initial values:
 *            a(0)=1, a(1)=8, a(2)=57, a(3)=406, a(4)=2892, a(5)=20600,
 *            a(6)=146736, a(7)=1045216
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small n directly with the known initial terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)57;
    if (n == 3) return (seq_t)406;
    if (n == 4) return (seq_t)2892;
    if (n == 5) return (seq_t)20600;
    if (n == 6) return (seq_t)146736;
    if (n == 7) return (seq_t)1045216;

    /* Sliding‑window variables for the last eight values:
       a0 = a(i-8), a1 = a(i-7), … , a7 = a(i-1)   */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)8;
    seq_t a2 = (seq_t)57;
    seq_t a3 = (seq_t)406;
    seq_t a4 = (seq_t)2892;
    seq_t a5 = (seq_t)20600;
    seq_t a6 = (seq_t)146736;
    seq_t a7 = (seq_t)1045216;
    seq_t xn;

    for (idx_t i = 8; i <= n; ++i)
    {
        xn = 8 * a7 - 7 * a6 + 6 * a5 - 5 * a4 + 4 * a3 - 3 * a2 + 2 * a1 - a0;
        /* shift the window for the next iteration */
        a0 = a1; a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = a6; a6 = a7; a7 = xn;
    }

    return a7;
}
