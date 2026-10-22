/*
 * A350473  a(n) = Fibonacci(n+1)^3 - Fibonacci(n-1)^3.
 * https://oeis.org/A350473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A350473
   a(n) = Fibonacci(n+1)^3 - Fibonacci(n‑1)^3
   Recurrence: a(n) = 3*a(n‑1) + 6*a(n‑2) – 3*a(n‑3) – a(n‑4)
   Initial values: a(0)=0, a(1)=1, a(2)=7, a(3)=26
*/

seq_t a(idx_t n)
{
    /* Handle the four base cases directly. */
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)7;
    if (n == 3u) return (seq_t)26;

    /* Keep the last four values in a sliding window. */
    seq_t a0 = (seq_t)0;   /* a(n‑4) */
    seq_t a1 = (seq_t)1;   /* a(n‑3) */
    seq_t a2 = (seq_t)7;   /* a(n‑2) */
    seq_t a3 = (seq_t)26;  /* a(n‑1) */

    /* Compute in a forward loop. */
    for (idx_t i = 4u; i <= n; ++i)
    {
        seq_t part1 = 3 * a3 + 6 * a2;        /* 3*a(n‑1)+6*a(n‑2) */
        seq_t part2 = 3 * a1 + a0;            /* 3*a(n‑3)+a(n‑4) */
        seq_t new_a = part1 - part2;          /* a(n) */

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = new_a;
    }

    return a3;   /* a(n) after the loop */
}
