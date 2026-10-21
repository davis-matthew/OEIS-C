/*
 * A288663  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 493", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-2) - 14*a(n-4) + 8*a(n-6) for n>6 with a(0)=1,
 *            a(1)=2, a(2)=1, a(3)=12, a(4)=3, a(5)=56, a(6)=7
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* Initial values for the first seven terms. */
    static const seq_t init[7] = {1, 2, 1, 12, 3, 56, 7};

    if (n <= 6)
        return init[n];

    /* Allocate an array of size n+1.  Use malloc in C and new[] in C++. */
    seq_t *arr = NULL;
#ifdef __cplusplus
    arr = new (std::nothrow) seq_t[n + 1];
    if (!arr) abort();
#else
    arr = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!arr) abort();
#endif

    /* Fill the known initial terms. */
    for (idx_t i = 0; i <= 6; ++i)
        arr[i] = init[i];

    /* Recurrence: a(i) = 7*a(i-2) - 14*a(i-4) + 8*a(i-6)  for i>6 */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t v2 = arr[i - 2];
        seq_t v4 = arr[i - 4];
        seq_t v6 = arr[i - 6];

        arr[i] = (seq_t)7 * v2 - (seq_t)14 * v4 + (seq_t)8 * v6;
    }

    seq_t result = arr[n];

    /* Free the allocated memory. */
#ifdef __cplusplus
    delete[] arr;
#else
    free(arr);
#endif

    return result;
}
