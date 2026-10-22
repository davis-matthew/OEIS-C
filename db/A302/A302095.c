/*
 * A302095  a(n) is the smallest positive integer not yet in the increasing
 *          sequence that is obtained when the largest digit from a(n-1) is
 *          deleted and the remaining digits are permuted such that no digit
 *          in a(n) has the same position it had in a(n-1) (counting from left
 *          to right). No repeated digits allowed...
 * https://oeis.org/A302095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is defined by recurrence: a(1)=10. For n>1, a(n) is the
 *            smallest positive integer not yet in the sequence that can be
 *            formed from the digits of a(n-1) after deleting the largest
 *            digit and permuting the remaining digits (no digit stays in its
 *            original position). New digits may be inserted as...
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Convert a number to its decimal digits (most significant first).   */
static void get_digits(seq_t x, uint8_t *buf, size_t *len)
{
    if (x == (seq_t)0) {
        buf[0] = 0;
        *len = 1;
        return;
    }
    size_t l = 0;
    seq_t t = x;
    while (t > (seq_t)0) {
        buf[l++] = (uint8_t)(t % 10);
        t /= 10;
    }
    for (size_t i = 0; i < l / 2; ++i) {
        uint8_t tmp = buf[i];
        buf[i] = buf[l - 1 - i];
        buf[l - 1 - i] = tmp;
    }
    *len = l;
}

/* ------------------------------------------------------------------ */
/* Recursive generation of permutations that satisfy the constraints. */
static void permute_rec(int pos, int L, const int *digits, const bool *isReq,
                        const uint8_t *posPrev, seq_t prev, seq_t *best,
                        seq_t cur, bool *used)
{
    if (pos == L) {
        if (cur > prev) {
            if (*best == (seq_t)0 || cur < *best)
                *best = cur;
        }
        return;
    }
    for (int i = 0; i < L; ++i) {
        if (used[i])
            continue;
        int d = digits[i];
        if (pos == 0 && d == 0)          /* leading zero not allowed */
            continue;
        if (isReq[d] && posPrev[d] == pos) /* same position as before */
            continue;
        used[i] = true;
        permute_rec(pos + 1, L, digits, isReq, posPrev, prev, best,
                    cur * (seq_t)10 + d, used);
        used[i] = false;
    }
}

/* ------------------------------------------------------------------ */
/* Find the next term given the previous term. */
static seq_t find_next(seq_t prev)
{
    uint8_t prev_digits[10];
    size_t lenPrev;
    get_digits(prev, prev_digits, &lenPrev);

    uint8_t posPrev[10];
    for (int d = 0; d < 10; ++d)
        posPrev[d] = 255;                     /* sentinel for absent */

    uint8_t maxDigit = 0;
    for (size_t i = 0; i < lenPrev; ++i) {
        uint8_t d = prev_digits[i];
        posPrev[d] = (uint8_t)i;
        if (d > maxDigit)
            maxDigit = d;
    }

    /* Required digits (all except the largest) */
    bool isReq[10] = {false};
    int reqDigits[10];
    int reqCnt = 0;
    for (size_t i = 0; i < lenPrev; ++i) {
        uint8_t d = prev_digits[i];
        if (d != maxDigit && !isReq[d]) {
            isReq[d] = true;
            reqDigits[reqCnt++] = d;
        }
    }
    /* Sort required digits for deterministic permutation order */
    for (int i = 1; i < reqCnt; ++i) {
        int key = reqDigits[i];
        int j = i - 1;
        while (j >= 0 && reqDigits[j] > key) {
            reqDigits[j + 1] = reqDigits[j];
            --j;
        }
        reqDigits[j + 1] = key;
    }

    /* Available digits (excluding the largest) */
    int avail[9];
    int availCnt = 0;
    for (int d = 0; d < 10; ++d) {
        if ((uint8_t)d != maxDigit) {
            avail[availCnt++] = d;
        }
    }

    /* Optional digits: available digits that are not required */
    int opt[9];
    int optCnt = 0;
    for (int i = 0; i < availCnt; ++i) {
        int d = avail[i];
        if (!isReq[d])
            opt[optCnt++] = d;
    }

    seq_t best = 0; /* 0 means not found yet */

    /* Search lengths from minimal required up to 10 */
    for (int L = reqCnt; L <= 10; ++L) {
        int need = L - reqCnt;
        if (need > optCnt)
            continue;

        int maxMask = 1 << optCnt;
        for (int mask = 0; mask < maxMask; ++mask) {
            if (__builtin_popcount((unsigned)mask) != need)
                continue;

            int digitsSize = reqCnt + need;
            int digits[10];
            for (int i = 0; i < reqCnt; ++i)
                digits[i] = reqDigits[i];
            int idx = reqCnt;
            for (int i = 0; i < optCnt; ++i) {
                if (mask & (1 << i))
                    digits[idx++] = opt[i];
            }

            /* Insertion sort for small array */
            for (int i = 1; i < digitsSize; ++i) {
                int key = digits[i];
                int j = i - 1;
                while (j >= 0 && digits[j] > key) {
                    digits[j + 1] = digits[j];
                    --j;
                }
                digits[j + 1] = key;
            }

            bool used[10] = {false};
            permute_rec(0, digitsSize, digits, isReq, posPrev, prev,
                        &best, (seq_t)0, used);
        }
        if (best != (seq_t)0)
            return best; /* minimal for this length */
    }

    return (seq_t)0; /* No further term (sequence ends after 59 terms) */
}

/* ------------------------------------------------------------------ */
/* Public function: compute the n-th term (n >= 1). */
seq_t a(idx_t n)
{
    /* The sequence has only 59 defined terms.  The remainder are 0. */
    static seq_t cache[60];
    static bool initialized = false;
    static idx_t computed = 1;

    if (!initialized) {
        cache[1] = (seq_t)10;   /* a(1) */
        initialized = true;
    }

    if (n > 59)
        return (seq_t)0;        /* undefined beyond 59 */

    while (computed < n) {
        cache[computed + 1] = find_next(cache[computed]);
        ++computed;
    }

    return cache[n];
}
