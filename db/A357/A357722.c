/*
 * A357722  Number of partitions of n into 4 distinct positive Fibonacci
 *          numbers (with a single type of 1).
 * https://oeis.org/A357722
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 4 distinct positive
 *            Fibonacci numbers (with only one 1)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the partitions of a given n.  The function is evaluated for all n ≥ 11.
   For n < 11 the result is 0 as it is below the OEIS offset.  */
seq_t a(idx_t n)
{
    /* If n is below the known offset simply return 0 – the behaviour is undefined
       for such n, but returning 0 avoids unnecessary work. */
    if (n < 11) {
        return 0;
    }

    /* Generate the distinct Fibonacci numbers up to n.
       We keep only one copy of 1, so the list starts: 1, 2, 3, 5, 8, …   */
    idx_t fib[200];          /* ample space for all Fibs ≤ 10 000 (to be safe) */
    idx_t len = 2;
    fib[0] = 1;
    fib[1] = 2;
    while (len < 200) {
        idx_t next = fib[len - 1] + fib[len - 2];
        if (next > n) break;
        fib[len++] = next;
    }

    seq_t count = 0;

    /* Iterate over all increasing 4‑tuples of indices (i, j, k, l).  */
    for (idx_t i = 0; i + 3 < len; ++i) {
        /* Prune the loops early using the fact that the array is sorted.  */
        if (fib[i] + fib[i + 1] + fib[i + 2] + fib[i + 3] > n) break;

        for (idx_t j = i + 1; j + 2 < len; ++j) {
            if (fib[i] + fib[j] + fib[j + 1] + fib[j + 2] > n) break;

            for (idx_t k = j + 1; k + 1 < len; ++k) {
                if (fib[i] + fib[j] + fib[k] + fib[k + 1] > n) break;

                for (idx_t l = k + 1; l < len; ++l) {
                    idx_t sum = fib[i] + fib[j] + fib[k] + fib[l];
                    if (sum == n) {
                        count++;
                    } else if (sum > n) {
                        /* As fib is increasing, further l will only increase the sum. */
                        break;
                    }
                }
            }
        }
    }

    return count;
}
