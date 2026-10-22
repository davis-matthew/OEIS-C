/*
 * A359663  a(1) = 1; for n > 1, a(n) is the smallest positive number which
 *          has not appeared that shares a factor with the sum of the first n
 *          terms of the Champernowne string starting from 1.
 * https://oeis.org/A359663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) for OEIS sequence A359663:
 *  a(1) = 1; for n > 1, a(n) is the smallest positive integer not
 *  yet used that shares a common divisor with the sum of the first n
 *  terms of the Champernowne string starting from 1 (digits 1,2,3,...,9,1,0,1,1,...).
 *  This implementation stores cumulative digit sums, a bitmap of used numbers,
 *  and the sequence itself, all as static arrays that grow on demand.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"

/*--------- cumulative sums of Champernowne digits ----------------*/
static seq_t *cumSum = NULL;   /* cumulative sum up to each digit   */
static size_t  cumCap = 0;     /* allocated capacity                */
static size_t  cumSize = 0;    /* number of digits processed so far */

/* next integer whose decimal representation will be appended */
static uint64_t nextNum = 1;

/* ensure cumSum can hold at least n entries (i.e. n digits) */
static void ensure_digits_up_to(idx_t n)
{
    while (cumSize < (size_t)n) {
        /* produce the next integer's decimal digits */
        uint64_t t = nextNum;
        unsigned char tmp[20];
        int len = 0;
        if (t == 0) tmp[len++] = 0;
        while (t > 0) {
            tmp[len++] = (unsigned char)(t % 10);
            t /= 10;
        }
        /* append digits in normal order */
        for (int i = len - 1; i >= 0; --i) {
            unsigned char d = tmp[i];

            /* grow cumSum array if needed */
            if (cumSize >= cumCap) {
                size_t newCap = cumCap ? cumCap * 2 : 16;
                seq_t *newC = (seq_t *)realloc(cumSum, newCap * sizeof(seq_t));
                if (!newC) abort();
                cumSum = newC;
                cumCap = newCap;
            }

            /* store new cumulative sum */
            seq_t prev = (cumSize > 0 ? cumSum[cumSize - 1] : 0);
            cumSum[cumSize++] = prev + (seq_t)d;
        }
        ++nextNum;
    }
}

/*--------- used‑number bitmap -----------------------------------*/
static unsigned char *used = NULL; /* 0 = unused, 1 = used */
static size_t  usedCap = 0;        /* current capacity of used array */

/* ensure used array can index up to v (inclusive) */
static void ensure_used_capacity(idx_t v)
{
    if ((size_t)v < usedCap) return;
    size_t newCap = usedCap ? usedCap * 2 : 16;
    while (newCap <= (size_t)v) newCap <<= 1;
    unsigned char *newU = (unsigned char *)realloc(used, newCap);
    if (!newU) abort();
    memset(newU + usedCap, 0, newCap - usedCap);
    used = newU;
    usedCap = newCap;
}

/*--------- memoized sequence a(n) -------------------------------*/
static seq_t *seqA = NULL; /* sequence values, 0‑based index = a(n+1) */
static size_t  aCap = 0;   /* capacity of seqA */
static size_t  aSize = 0;  /* number of terms computed so far */

/* ensure seqA can hold at least n elements */
static void ensure_seqA_capacity(size_t n)
{
    if (n <= aCap) return;
    size_t newCap = aCap ? aCap * 2 : 16;
    while (newCap < n) newCap <<= 1;
    seq_t *newA = (seq_t *)realloc(seqA, newCap * sizeof(seq_t));
    if (!newA) abort();
    seqA = newA;
    aCap = newCap;
}

/*--------- required function ------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* safety, not used by OEIS */

    /* ensure at least term 1 is defined */
    if (aSize == 0) {
        ensure_used_capacity(1);
        used[1] = 1;
        ensure_seqA_capacity(1);
        seqA[0] = 1;
        aSize = 1;
    }

    /* if the requested index is already computed, just return it */
    if ((size_t)n <= aSize)
        return seqA[n - 1];

    /* compute terms up to n */
    for (idx_t cur = aSize + 1; cur <= n; ++cur) {
        /* obtain the sum of the first cur digits */
        ensure_digits_up_to(cur);
        seq_t sum = cumSum[cur - 1];

        /* find the smallest unused candidate sharing a factor with sum */
        seq_t cand = 1;
        while (1) {
            /* skip if already used */
            if (!(cand < (seq_t)usedCap && used[(size_t)cand])) {
                if (gcd(sum, cand) > 1) {
                    /* mark as used and store */
                    ensure_used_capacity((idx_t)cand);
                    used[(size_t)cand] = 1;
                    ensure_seqA_capacity(aSize + 1);
                    seqA[aSize++] = cand;
                    break;
                }
            }
            ++cand;
        }
    }

    return seqA[n - 1];
}
