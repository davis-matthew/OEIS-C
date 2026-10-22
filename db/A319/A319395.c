/*
 * A319395  Number of partitions of n into exactly two positive Fibonacci
 *          numbers.
 * https://oeis.org/A319395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs of positive Fibonacci numbers
 *            whose sum is n (count the solutions to n = f + g with f,g ∈
 *            {1,2,3,5,8,…}, f ≤ g)
 * Range:     u8 n <= 1582, u16 n <= 9698, u32 n <= 9698, u64 n <= 9698,
 *            u128 n <= 9698, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Number of partitions of n into exactly two positive Fibonacci numbers */
seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n == 0) return 0;

    /* Lazy static storage of distinct positive Fibonacci numbers */
    static seq_t *fib = NULL;        /* array of Fibonacci values */
    static size_t fib_cnt = 0;       /* number of entries stored */
    static size_t fib_cap = 0;       /* current capacity of the array */

    /* Initialize the Fibonacci array if needed */
    if (fib == NULL) {
        fib_cap = 16;
        fib = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        if (fib == NULL) return 0;  /* allocation failure: return 0 */
        /* Distinct positive Fibonacci numbers start: 1, 2 */
        fib[0] = 1;
        fib[1] = 2;
        fib_cnt = 2;
    }

    /* Extend the Fibonacci array until it contains all fib ≤ n */
    while (fib[fib_cnt - 1] < (seq_t) n) {
        if (fib_cnt == fib_cap) {
            fib_cap <<= 1; /* double capacity */
            seq_t *tmp = (seq_t *)realloc(fib, fib_cap * sizeof(seq_t));
            if (tmp == NULL) return 0; /* allocation failure */
            fib = tmp;
        }
        fib[fib_cnt] = fib[fib_cnt - 1] + fib[fib_cnt - 2];
        fib_cnt++;
    }

    /* Count unordered pairs (f,g) with f ≤ g, f+g = n */
    seq_t count = 0;
    seq_t limit = (seq_t)(n / 2);
    for (size_t i = 0; i < fib_cnt; i++) {
        seq_t f = fib[i];
        if (f > limit) break;
        seq_t g = (seq_t)n - f;

        /* Binary search in fib array for g */
        size_t low = 0, high = fib_cnt;
        while (low < high) {
            size_t mid = (low + high) >> 1;
            if (fib[mid] == g) {
                count++;
                break;
            } else if (fib[mid] < g) {
                low = mid + 1;
            } else {
                high = mid;
            }
        }
    }
    return count;
}
