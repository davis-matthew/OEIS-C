/*
 * A291383  p-INVERT of (1,1,0,0,0,0,...), where p(S) = 1 - 2 S - 2 S^2.
 * https://oeis.org/A291383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) + 4*a(n-3) + 2*a(n-4) for n>=5
 *            Initial values: a(0)=2, a(1)=8, a(2)=28, a(3)=98, a(4)=344
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
    case 0: return (seq_t)2;
    case 1: return (seq_t)8;
    case 2: return (seq_t)28;
    case 3: return (seq_t)98;
    case 4: return (seq_t)344;
    default:
        seq_t a1 = (seq_t)2;   // a(n-4)
        seq_t a2 = (seq_t)8;   // a(n-3)
        seq_t a3 = (seq_t)28;  // a(n-2)
        seq_t a4 = (seq_t)98;  // a(n-1)
        seq_t a5 = (seq_t)344; // a(n)
        /* compute up to n using recurrence */
        for (idx_t i = 5; i <= n; ++i) {
            seq_t next = 2*a5 + 4*a4 + 4*a3 + 2*a2;
            a2 = a3;
            a3 = a4;
            a4 = a5;
            a5 = next;
        }
        return a5;
    }
}
