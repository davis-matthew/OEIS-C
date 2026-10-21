/*
 * A251772  Number of (n+2) X (1+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum 3 or 6 and every diagonal and antidiagonal sum not
 *          3 or 6.
 * https://oeis.org/A251772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 20, u64 n <= 46, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(n) = a(n-1) + 16*a(n-3) - 16*a(n-4) - 39*a(n-6) + 39*a(n-7)
          - 36*a(n-9) + 36*a(n-10);
   Initial values (n = 1 … 10):
   a(1)=470,  a(2)=1142,  a(3)=2402,  a(4)=4790,  a(5)=12302,
   a(6)=27242, a(7)=55766, a(8)=148022, a(9)=332738, a(10)=688646.
   The recurrence holds for all n≥11. */

#include <stdlib.h>

static const seq_t base[10] = {
    470ULL, 1142ULL, 2402ULL, 4790ULL, 12302ULL,
    27242ULL, 55766ULL, 148022ULL, 332738ULL, 688646ULL
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1 */

    /* Fast path for the first ten terms. */
    if (n <= 10) return base[(size_t)n - 1];

    /* Allocate space for all terms up to n. */
    size_t N = (size_t)n;
    seq_t *arr = (seq_t *)malloc(N * sizeof(seq_t));
    if (arr == NULL) return 0;           /* allocation failure */

    /* Initialise with the known base values. */
    for (size_t i = 0; i < 10; ++i) arr[i] = base[i];

    /* Compute the remaining terms using the recurrence. */
    for (size_t i = 10; i < N; ++i) {
        seq_t val = arr[i - 1];
        val += (seq_t)16 * arr[i - 3];
        val -= (seq_t)16 * arr[i - 4];
        val -= (seq_t)39 * arr[i - 6];
        val += (seq_t)39 * arr[i - 7];
        val -= (seq_t)36 * arr[i - 9];
        val += (seq_t)36 * arr[i -10];
        arr[i] = val;
    }

    seq_t result = arr[N - 1];
    free(arr);
    return result;
}
