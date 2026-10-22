/*
 * A348853  Delete any least significant 0's from the Zeckendorf
 *          representation of n, leaving its "odd" part.
 * https://oeis.org/A348853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   delete the least‑significant 0's from the Zeckendorf
 *            representation of n, leaving its “odd” part
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Fibonacci numbers 1,2,3,5,… stored in a static array. */
static seq_t fib[1024];
static size_t fib_len = 0;      /* number of Fibonacci values currently stored */

static void ensure_fib(seq_t n_max)
{
    if (fib_len == 0) {                    /* initialise first two Fibonacci numbers */
        fib_len = 2;
        fib[0] = (seq_t)1;
        fib[1] = (seq_t)2;
    }
    /* Grow the array while the last element <= n_max */
    while (fib[fib_len - 1] <= n_max && fib_len < 1024) {
        fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
        ++fib_len;
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑indexed; n ≥ 1 */
    seq_t N = (seq_t)n;                     /* cast to seq_t for arithmetic   */
    ensure_fib(N);

    /* Zeckendorf representation by greedy subtraction.
       Store the used indices in descending order. */
    uint32_t idxs[1024];
    size_t idx_cnt = 0;

    seq_t rem = N;
    for (size_t i = fib_len; i-- > 0;) {
        if (rem >= fib[i]) {
            rem -= fib[i];
            idxs[idx_cnt++] = (uint32_t)i;
        }
    }
    /* idx_cnt ≥ 1 because n ≥ 1 */

    /* Smallest used index = trailing zeros count (0‑based). */
    uint32_t shift = idxs[idx_cnt - 1];

    /* Compute the odd part: shift all indices down by `shift`. */
    seq_t result = (seq_t)0;
    for (size_t j = 0; j < idx_cnt; ++j) {
        size_t shifted = idxs[j] - shift;
        result += fib[shifted];
    }
    return result;
}
