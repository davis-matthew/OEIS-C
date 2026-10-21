/*
 * A255675  Least integer k such that the n-th prime of form m^2+1 divides the
 *          composite number k^2+1.
 * https://oeis.org/A255675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 for n=1; a(n) = A002496(n)-A005574(n) for n>1 = m_n^2 -
 *            m_n + 1 where m_n is the n-th integer with m_n^2+1 prime
 * Range:     u8 n <= 6, u16 n <= 37, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* static state for memoisation */
    static bool initialized = false;
    static seq_t *answers = NULL;      /* 0‑based array of a-values */
    static size_t capacity = 0;        /* allocated size of answers */
    static size_t len = 0;             /* number of a-values stored */
    static uint64_t last_m = 1;        /* last m that was tested */

    if (!initialized) {
        /* initialise cache with the first term a(1)=3 */
        capacity = 1;
        answers = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!answers) return 0; /* memory failure – unlikely in tests */
        answers[0] = 3;
        len = 1;
        initialized = true;
    }

    /* generate terms up to n if needed */
    while (len < n) {
        /* test next m */
        ++last_m;
        seq_t m = (seq_t)last_m;
        seq_t prime_candidate = m * m + 1;
        if (is_prime_big(prime_candidate)) {
            /* compute a-value for this m */
            seq_t val = m * m - m + 1;
            /* grow array if necessary */
            if (len == capacity) {
                capacity = capacity * 2 ? capacity * 2 : 2;
                seq_t *tmp = (seq_t *)realloc(answers, sizeof(seq_t) * capacity);
                if (!tmp) return 0; /* memory failure */
                answers = tmp;
            }
            answers[len++] = val;
        }
    }
    return answers[n - 1];
}
