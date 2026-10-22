/*
 * A354430  First diagonal of an array, generated from the sequence of the
 *          nonprimes.
 * https://oeis.org/A354430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{n-1} C(n-1,k) * nonprime(2k+1) (nonprime(1)=1)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for nonprime numbers */
static seq_t *nonprimes = NULL;   /* array holding nonprimes up to a certain limit */
static idx_t nonprime_cnt = 0;    /* current number of nonprimes stored */

/* Ensure that nonprimes[0 … need-1] exists (need >= 1) */
static void ensure_nonprimes(idx_t need)
{
    if (nonprime_cnt >= need)
        return;

    /* Dynamic growth: start with a modest bound and double until enough nonprimes */
    idx_t limit = 20;
    while (true) {
        /* Simple sieve of Eratosthenes up to 'limit' */
        unsigned char *is_comp = (unsigned char *)calloc((size_t)limit + 1, 1);
        if (!is_comp) {
            /* Allocation failed – abort */
            exit(EXIT_FAILURE);
        }
        is_comp[0] = is_comp[1] = 1;          /* 0 and 1 are not prime (treated as composite) */
        for (idx_t p = 2; p * p <= limit; ++p) {
            if (!is_comp[p]) {
                for (idx_t q = p * p; q <= limit; q += p)
                    is_comp[q] = 1;
            }
        }

        /* Count nonprimes up to limit */
        idx_t cnt = 0;
        for (idx_t i = 1; i <= limit; ++i) {
            if (i == 1 || is_comp[i])
                ++cnt;
        }

        if (cnt >= need) {
            /* Allocate and fill the array of nonprimes */
            seq_t *new_arr = (seq_t *)malloc((size_t)cnt * sizeof(seq_t));
            if (!new_arr) {
                free(is_comp);
                exit(EXIT_FAILURE);
            }
            idx_t idx = 0;
            for (idx_t i = 1; i <= limit; ++i) {
                if (i == 1 || is_comp[i])
                    new_arr[idx++] = (seq_t)i;
            }
            free(nonprimes);
            nonprimes = new_arr;
            nonprime_cnt = cnt;
            free(is_comp);
            return;
        }

        /* Not enough nonprimes – increase limit and try again */
        free(is_comp);
        limit *= 2;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* safety guard – not part of OEIS offset */
        return 0;

    idx_t needed = 2 * n - 1;          /* number of nonprimes required */
    ensure_nonprimes(needed);          /* guarantee existence of nonprimes[0 … needed-1] */

    seq_t result = 0;
    seq_t coeff  = 1;                  /* C(n-1,0) */

    for (idx_t k = 0; k < n; ++k) {
        seq_t val = nonprimes[(size_t)(2 * k)];   /* nonprime(2k+1) */
        result += coeff * val;

        if (k + 1 < n) {
            /* Update coeff to C(n-1, k+1) */
            coeff = (coeff * (seq_t)(n - 1 - k)) / (seq_t)(k + 1);
        }
    }

    return result;
}
