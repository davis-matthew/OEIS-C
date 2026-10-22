/*
 * A384294  The number of Hamiltonian cycles in the concentric ring graph of
 *          order n.
 * https://oeis.org/A384294
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*Lucas(n) - 2 + 2*n*[Mod(n,3)==2]
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for Lucas numbers */
static seq_t *lucas = NULL;
static idx_t lucas_size = 0;

/* Compute Lucas number L(n) with memoization */
static seq_t lucas_n(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;

    /* Initialise cache if necessary */
    if (lucas == NULL) {
        lucas = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!lucas) return (seq_t)0; /* allocation failed – return 0 */
        lucas[0] = (seq_t)2;
        lucas[1] = (seq_t)1;
        lucas_size = 2;
    }

    /* Grow cache up to n */
    if ((idx_t)lucas_size <= n) {
        size_t old_size = (size_t)lucas_size;
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(lucas, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed – return 0 */
        lucas = tmp;
        lucas_size = (idx_t)new_size;
        for (idx_t i = old_size; i <= n; ++i) {
            lucas[i] = lucas[i-1] + lucas[i-2];
        }
    }

    return lucas[n];
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* For n < 3 the sequence is undefined; return 0 */
    if (n < 3) return (seq_t)0;

    seq_t Ln = lucas_n(n);
    seq_t result = (seq_t)2 * Ln - (seq_t)2;
    if (n % 3 == 2) {
        result += (seq_t)2 * (seq_t)n;
    }
    return result;
}
