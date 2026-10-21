/*
 * A257692  Numbers such that the smallest nonzero digit present (A257679) in
 *          their factorial base representation is 2.
 * https://oeis.org/A257692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k with A257679(k) = 2 (i.e. the smallest
 *            non-zero digit in the factorial base representation of k is 2).
 *            This implementation searches integers sequentially and stores
 *            results in a static cache
 * Range:     u8 n <= 18, u16 n <= 4320, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Precomputed factorials up to 20!; 20! fits in all supported
   integer sizes and suffices for the required range (n ≤ 10000). */
#define FACT_SIZE 21
static const seq_t FACT[FACT_SIZE] = {
    1ULL,                     /* 0!  (unused) */
    1ULL,                     /* 1!  */
    2ULL,                     /* 2!  */
    6ULL,                     /* 3!  */
    24ULL,                    /* 4!  */
    120ULL,                   /* 5!  */
    720ULL,                   /* 6!  */
    5040ULL,                  /* 7!  */
    40320ULL,                 /* 8!  */
    362880ULL,                /* 9!  */
    3628800ULL,               /* 10! */
    39916800ULL,              /* 11! */
    479001600ULL,             /* 12! */
    6227020800ULL,            /* 13! */
    87178291200ULL,           /* 14! */
    1307674368000ULL,         /* 15! */
    20922789888000ULL,        /* 16! */
    355687428096000ULL,       /* 17! */
    6402373705728000ULL,      /* 18! */
    121645100408832000ULL,    /* 19! */
    2432902008176640000ULL    /* 20! */
};

/* Cache for computed sequence values; sufficient for up to 20000 terms. */
#define MAX_TERMS 20000
static seq_t SEQ_CACHE[MAX_TERMS];
static size_t SEQ_LEN = 0;

/* Determine whether num has no digit '1' in its factorial base
   representation and contains at least one digit '2'. */
static bool has_smallest_nonzero_digit_two(seq_t num)
{
    bool has_two = false;
    /* Find largest factorial <= num. */
    size_t k = 1;
    while (k < FACT_SIZE && FACT[k] <= num) {
        ++k;
    }
    --k;  /* kth factorial is ≤ num. */

    seq_t rem = num;
    for (size_t i = k; i > 0; --i) {
        seq_t f     = FACT[i];
        seq_t digit = rem / f;
        rem -= digit * f;
        if (digit != 0) {
            if (digit == 1) {
                return false;      /* digit 1 disqualifies */
            }
            if (digit == 2) {
                has_two = true;
            }
        }
    }
    return has_two;
}

/* Public function: returns the n-th member of sequence A257692. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;  /* undefined for n = 0; return 0 as a safeguard */
    }

    /* Return from cache if already computed. */
    if ((size_t)n <= SEQ_LEN) {
        return SEQ_CACHE[n - 1];
    }

    /* If cache is full, continue computing without storing to avoid overflow. */
    bool store_in_cache = (SEQ_LEN < MAX_TERMS);

    /* Start searching from the next integer after the last cached value. */
    seq_t cand = (SEQ_LEN == 0) ? 1 : SEQ_CACHE[SEQ_LEN - 1] + 1;

    while (SEQ_LEN < (size_t)n) {
        if (has_smallest_nonzero_digit_two(cand)) {
            if (store_in_cache) {
                SEQ_CACHE[SEQ_LEN++] = cand;
            } else {
                /* Cache full: keep advancing without storing. */
                ++SEQ_LEN;
            }
        }
        ++cand;
    }

    /* Return the desired term. */
    return SEQ_CACHE[n - 1];
}
