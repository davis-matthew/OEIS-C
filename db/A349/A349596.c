/*
 * A349596  Number of edge subsets E of the 3 X n grid graph such that E
 *          contains a path between the top left node and the bottom right
 *          node.
 * https://oeis.org/A349596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A349596: Number of edge subsets E of the 3 × n grid graph
   such that E contains a path between the top left node and the bottom
   right node.
   Recurrence (for n ≥ 10):
     a(n) = 54*a(n-1) - 1077*a(n-2) + 10642*a(n-3) - 57954*a(n-4)
            + 180960*a(n-5) - 324992*a(n-6) + 325632*a(n-7)
            - 165888*a(n-8) + 32768*a(n-9)
   Base values:
     a(1)=1, a(2)=40, a(3)=1135, a(4)=28942, a(5)=707239,
     a(6)=16963938, a(7)=403490839,
     a(8)=9560192914, a(9)=226115020735 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0. */
    if (n < 1) return (seq_t)0;

    /* Base terms for 1 ≤ n ≤ 9.  Use 64‑bit literals to initialize. */
    static const seq_t base[9] = {
        (seq_t)1ULL,
        (seq_t)40ULL,
        (seq_t)1135ULL,
        (seq_t)28942ULL,
        (seq_t)707239ULL,
        (seq_t)16963938ULL,
        (seq_t)403490839ULL,
        (seq_t)9560192914ULL,
        (seq_t)226115020735ULL
    };

    if (n <= 9) return base[n - 1];

    /* Keep the last nine values: a(i-9) … a(i-1).  Initially they are
       a(1)…a(9). */
    seq_t a1 = base[0]; /* a(i-9)   */
    seq_t a2 = base[1]; /* a(i-8)   */
    seq_t a3 = base[2]; /* a(i-7)   */
    seq_t a4 = base[3]; /* a(i-6)   */
    seq_t a5 = base[4]; /* a(i-5)   */
    seq_t a6 = base[5]; /* a(i-4)   */
    seq_t a7 = base[6]; /* a(i-3)   */
    seq_t a8 = base[7]; /* a(i-2)   */
    seq_t a9 = base[8]; /* a(i-1)   */

    for (idx_t i = 10; i <= n; ++i) {
        seq_t newa =
            (seq_t)54ULL  * a9
          - (seq_t)1077ULL * a8
          + (seq_t)10642ULL * a7
          - (seq_t)57954ULL * a6
          + (seq_t)180960ULL * a5
          - (seq_t)324992ULL * a4
          + (seq_t)325632ULL * a3
          - (seq_t)165888ULL * a2
          + (seq_t)32768ULL  * a1;

        /* Shift the window: a1←a2, a2←a3, …, a8←a9, a9←newa. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = a8;
        a8 = a9;
        a9 = newa;
    }

    return a9; /* a(n) */
}
