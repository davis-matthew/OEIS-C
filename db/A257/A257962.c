/*
 * A257962  List of successive permutations of the numbers [0,F(n)) achieved
 *          by writing the numbers in the Fibonacci/Zeckendorf radix in the
 *          form of an (n-2)-bit string, reversing that string, and extracting
 *          the numerical value according to Fibonacci/Zeckendorf radix.
 * https://oeis.org/A257962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   a(n) = permutation obtained by reversing the Zeckendorf * digits
 *            of (n - cum[m-1]) inside its block m, where m is minimal * with
 *            cum[m-1] ≤ n < cum[m] and cum[k] = Σ_{i=1..k} F(i)
 * Range:     u8 n <= 611, u16 n <= 9427, u32 n <= 9427, u64 n <= 9427,
 *            u128 n <= 9427, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* maximum number of Fibonacci numbers we precompute (enough for n≈10⁴) */
#define MAX_FIB 128

static seq_t  fib[MAX_FIB]   = {0};
static idx_t  cum[MAX_FIB]   = {0};
static int    initialized = 0;

static void
init_fib(void)
{
    size_t i;
    /* Fibonacci numbers with F(1)=1, F(2)=2 */
    if (initialized) return;
    fib[1] = 1;
    fib[2] = 2;
    for (i = 3; i < MAX_FIB; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    cum[0] = 0;
    for (i = 1; i < MAX_FIB; ++i) {
        /* prevent unsigned overflow – only needed for very large indices */
        if (cum[i - 1] > (idx_t)(-1) - (idx_t)fib[i]) {
            cum[i] = (idx_t)(-1);
        } else {
            cum[i] = cum[i - 1] + (idx_t)fib[i];
        }
    }
    initialized = 1;
}

seq_t a(idx_t n)
{
    size_t m;          /* block index (1‑based) */
    idx_t pos;         /* position inside the block */
    seq_t result = 0;

    init_fib();

    /* Find the block m such that cum[m-1] ≤ n < cum[m] */
    m = 1;
    while (m < MAX_FIB && n >= cum[m]) {
        ++m;
    }

    /* position within the block */
    pos = n - cum[m - 1];

    /* If the block contains only one element, return 0 immediately */
    if (m <= 1) return 0;

    /* Greedy Zeckendorf decomposition of pos using fib[1..m-1] */
    /* For each set bit at index j (1-based) add fib[m-j] to the result */
    {
        size_t j;
        for (j = m - 1; j >= 1; --j) {
            /* Compare with idx_t cast – safe because fib[j] ≤ F(m-1) */
            if (pos >= (idx_t)fib[j]) {
                pos -= (idx_t)fib[j];
                result += fib[m - j];
            }
            if (j == 1) break;  /* avoid size_t underflow */
        }
    }

    return result;
}
