/*
 * A359512  Number of partitions of n into at most 3 positive Fibonacci
 *          numbers (with a single type of 1).
 * https://oeis.org/A359512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by enumerating Fibonacci numbers up to n and counting
   unordered pairs and triples of such numbers whose sum is n. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;  /* empty partition */

    /* build Fibonacci numbers ≤ n (1,2,3,5,8,…) */
    const unsigned FMAXSIZE = 40;          /* plenty for n ≤ 10 000 */
    seq_t fib[FMAXSIZE];
    idx_t fibcnt = 0;
    seq_t f1 = 1, f2 = 2;
    while (f1 <= (seq_t)n) {
        fib[fibcnt++] = f1;
        seq_t tmp = f1 + f2;
        f1 = f2;
        f2 = tmp;
    }

    seq_t result = 0;

    /* single part */
    for (idx_t i = 0; i < fibcnt; ++i)
        if (fib[i] == (seq_t)n) { result++; break; }

    /* unordered pairs */
    for (idx_t i = 0; i < fibcnt; ++i) {
        seq_t aVal = fib[i];
        for (idx_t j = i; j < fibcnt; ++j) {
            seq_t sum2 = aVal + fib[j];
            if (sum2 == (seq_t)n)
                result++;
            else if (sum2 > (seq_t)n)
                break;  /* later fib[j] are larger */
        }
    }

    /* unordered triples */
    for (idx_t i = 0; i < fibcnt; ++i) {
        seq_t aVal = fib[i];
        for (idx_t j = i; j < fibcnt; ++j) {
            seq_t abSum = aVal + fib[j];
            for (idx_t k = j; k < fibcnt; ++k) {
                seq_t sum3 = abSum + fib[k];
                if (sum3 == (seq_t)n)
                    result++;
                else if (sum3 > (seq_t)n)
                    break;  /* further fib[k] only increase sum */
            }
        }
    }

    return result;
}
