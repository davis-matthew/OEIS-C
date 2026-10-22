/*
 * A383209  Irregular triangle read by rows in which row n lists the odd
 *          divisors m of n such that there is a divisor d of n with d < m <
 *          2*d, or 0 if such odd divisors do not exist.
 * https://oeis.org/A383209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A383209 (OEIS seq. a(n)) */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {uint64_t p; int exp;} Factor;

/* Generate all divisors of a number given its prime factorisation */
static void gen_divs(const Factor *fac, int fidx, int fcnt, uint64_t cur,
                     uint64_t *arr, size_t *cnt)
{
    if (fidx == fcnt) {
        arr[(*cnt)++] = cur;
        return;
    }
    uint64_t val = cur;
    for (int e = 0; e <= fac[fidx].exp; ++e) {
        gen_divs(fac, fidx + 1, fcnt, val, arr, cnt);
        if (e < fac[fidx].exp)
            val *= fac[fidx].p;
    }
}

/* Simple insertion sort for small arrays */
static void insertion_sort(uint64_t *arr, size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        uint64_t key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

seq_t a(idx_t idx)
{
    idx_t target = idx;            /* 1‑based sequence index */
    idx_t cum = 0;                 /* number of terms seen so far */

    for (idx_t n = 1; ; ++n) {
        /* --- factorise n --- */
        uint64_t x = (uint64_t)n;
        Factor fac[16];
        int faccnt = 0;

        /* factor 2 */
        int e = 0;
        while ((x & 1ULL) == 0) {
            x >>= 1;
            ++e;
        }
        if (e > 0) {
            fac[faccnt].p = 2;
            fac[faccnt].exp = e;
            ++faccnt;
        }

        /* odd factors */
        for (uint64_t p = 3; p * p <= x; p += 2) {
            if (x % p == 0) {
                e = 0;
                while (x % p == 0) {
                    x /= p;
                    ++e;
                }
                fac[faccnt].p = p;
                fac[faccnt].exp = e;
                ++faccnt;
            }
        }
        if (x > 1) {
            fac[faccnt].p = x;
            fac[faccnt].exp = 1;
            ++faccnt;
        }

        /* --- generate divisors --- */
        uint64_t divs[256];
        size_t divcnt = 0;
        gen_divs(fac, 0, faccnt, 1ULL, divs, &divcnt);
        insertion_sort(divs, divcnt);

        /* --- collect valid odd divisors --- */
        uint64_t rowVals[256];
        size_t rowcnt = 0;
        for (size_t i = 0; i < divcnt; ++i) {
            uint64_t d = divs[i];
            if ((d & 1ULL) == 0)          /* skip even divisors */
                continue;

            /* find a divisor in (d/2, d) */
            uint64_t threshold = (d >> 1) + 1;   /* floor(d/2)+1 */
            size_t l = 0, r = divcnt;
            while (l < r) {
                size_t mid = (l + r) >> 1;
                if (divs[mid] < threshold)
                    l = mid + 1;
                else
                    r = mid;
            }
            if (l < divcnt && divs[l] < d) {
                rowVals[rowcnt++] = d;
            }
        }

        /* --- decide if target lies in this row --- */
        if (rowcnt == 0) {               /* row consists solely of 0 */
            if (cum + 1 >= target)
                return (seq_t)0;
            cum += 1;
        } else {
            if (cum + rowcnt >= target) {
                size_t k = target - cum;  /* 1‑based position in row */
                return (seq_t)rowVals[k - 1];
            }
            cum += rowcnt;
        }
    }
    /* unreachable; keeps the function type-consistent */
    return (seq_t)0;
}
