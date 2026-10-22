/*
 * A342235  Coordination sequence of David Eppstein's "Tetrastix" graph.
 * https://oeis.org/A342235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 147, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   The coordination sequence a(n) for David Eppstein's "Tetrastix" graph.
   a(0) = 1
   a(1) = 4
   a(2) = 12
   a(3) = 28
   a(4) = 50
   For n >= 5:
       k = floor((n-3)/2)
       l = floor((n-4)/2)
       a(n) = 50 + 6*k*k + 20*k + 6*l*l + 28*l
*/

seq_t a(idx_t n)
{
    /* Handle the first five values explicitly */
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)4;
        case 2: return (seq_t)12;
        case 3: return (seq_t)28;
        case 4: return (seq_t)50;
    }

    /* n >= 5 */
    idx_t k = (n - 3) / 2;   /* floor((n-3)/2) */
    idx_t l = (n - 4) / 2;   /* floor((n-4)/2) */

    seq_t sk = (seq_t)k;
    seq_t sl = (seq_t)l;

    seq_t result = (seq_t)50;

    result += ((seq_t)6 * sk * sk);
    result += ((seq_t)20 * sk);
    result += ((seq_t)6 * sl * sl);
    result += ((seq_t)28 * sl);

    return result;
}
