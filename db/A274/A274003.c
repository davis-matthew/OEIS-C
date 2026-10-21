/*
 * A274003  Primitive terms (not equal twice a smaller term) of A178751:
 *          moduli n such that x^y == -1 (mod n) only for x = -1 (mod n).
 * https://oeis.org/A274003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS A274003.
    The sequence consists of:
        2,
        odd terms of the form 3 * Π p,
        even terms of the form 4 * Π p,
    where the product runs over any subset of the known Fermat
    primes excluding 3 (namely 5, 17, 257, 65537).
    Terms that are exactly twice a smaller term are omitted.
    The result is sorted in ascending order and returned for the
    requested index (1‑based).  */
//  (The definition relies on the known Fermat primes.
#include <stdlib.h>
#include <stdbool.h>

/* comparison for qsort of seq_t values */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* known Fermat primes: 3, 5, 17, 257, 65537 */
static const seq_t fermat_primes[5] = {
    (seq_t)3,
    (seq_t)5,
    (seq_t)17,
    (seq_t)257,
    (seq_t)65537
};

seq_t a(idx_t n)
{
    /* number of known Fermat primes (including 3) */
    const size_t total_primes     = sizeof(fermat_primes) / sizeof(fermat_primes[0]);
    const size_t primes_excl_start = 1;                        /* skip 3 */
    const size_t primes_excl_count = total_primes - primes_excl_start;  /* 4 for current data */

    /* maximum number of generated terms: 1 (for 2) + 2 * 2^primes_excl_count */
    const size_t max_terms = 1 + 2 * ((size_t)1 << primes_excl_count);

    seq_t tmp[max_terms];
    size_t tmp_cnt = 0;

    /* include the trivial term 2 */
    tmp[tmp_cnt++] = (seq_t)2;

    /* generate odd terms: 3 * product over any subset of the other Fermat primes */
    const size_t subset_limit = ((size_t)1 << primes_excl_count);
    for (size_t mask = 0; mask < subset_limit; ++mask) {
        seq_t prod = (seq_t)1;
        for (size_t i = 0; i < primes_excl_count; ++i) {
            if (mask & ((size_t)1 << i)) {
                prod *= fermat_primes[primes_excl_start + i];
            }
        }
        tmp[tmp_cnt++] = (seq_t)3 * prod;          /* odd term */
        tmp[tmp_cnt++] = (seq_t)4 * prod;          /* even term */
    }

    /* sort the generated numbers */
    qsort(tmp, tmp_cnt, sizeof(seq_t), seq_cmp);

    /* filter out terms that are exactly twice a smaller term */
    seq_t terms[max_terms];
    size_t terms_cnt = 0;
    for (size_t i = 0; i < tmp_cnt; ++i) {
        seq_t cur = tmp[i];
        bool skip = false;
        for (size_t j = 0; j < terms_cnt; ++j) {
            if (cur == (seq_t)2 * terms[j]) {
                skip = true;
                break;
            }
        }
        if (!skip) {
            terms[terms_cnt++] = cur;
        }
    }

    if (n == 0 || n > terms_cnt) {
        return (seq_t)0;  /* undefined for out of range */
    }
    return terms[n - 1];
}
