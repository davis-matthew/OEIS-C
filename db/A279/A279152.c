/*
 * A279152  Number of n X 2 0..1 arrays with no element equal to a strict
 *          majority of its king-move neighbors, with the exception of exactly
 *          one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A279152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A279152
 *
 * Empirical recurrence:
 *   a(n) = 4*a(n-1) - 5*a(n-2) + 6*a(n-3) - 12*a(n-4) 
 *          + 8*a(n-5) - 4*a(n-6) + 8*a(n-7)
 *
 * Initial values (offset 1):
 *   a(1) = 0
 *   a(2) = 0
 *   a(3) = 4
 *   a(4) = 12
 *   a(5) = 30
 *   a(6) = 72
 *   a(7) = 162
 *
 * The function evaluates the recurrence iteratively; no dynamic
 * allocation is required so it compiles under C11 as well as C++17
 * for any seq_t type that supports the usual arithmetic operators.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined below offset; return 0 */

    /* Direct return for the first seven terms */
    switch (n) {
        case 1: return (seq_t)0;
        case 2: return (seq_t)0;
        case 3: return (seq_t)4;
        case 4: return (seq_t)12;
        case 5: return (seq_t)30;
        case 6: return (seq_t)72;
        case 7: return (seq_t)162;
        default: break;
    }

    /* 7 rolling variables holding a(i-7) .. a(i-1) during the loop */
    seq_t a1 = (seq_t)0;   /* a(n-7) */
    seq_t a2 = (seq_t)0;   /* a(n-6) */
    seq_t a3 = (seq_t)4;   /* a(n-5) */
    seq_t a4 = (seq_t)12;  /* a(n-4) */
    seq_t a5 = (seq_t)30;  /* a(n-3) */
    seq_t a6 = (seq_t)72;  /* a(n-2) */
    seq_t a7 = (seq_t)162; /* a(n-1) */
    seq_t term;

    for (idx_t i = 8; i <= n; ++i) {
        /* a(i) = 4*a7 - 5*a6 + 6*a5 - 12*a4 + 8*a3 - 4*a2 + 8*a1 */
        term = 4 * a7;
        term -= 5 * a6;
        term += 6 * a5;
        term -= 12 * a4;
        term += 8 * a3;
        term -= 4 * a2;
        term += 8 * a1;

        /* shift window to prepare for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = term;
    }

    return a7;  /* a(n) is stored in a7 after the loop */
}
