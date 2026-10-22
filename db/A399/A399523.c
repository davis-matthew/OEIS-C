/*
 * A399523  Indices of the perfect powers (A001597) in the sequence of
 *          powerful numbers (A001694).
 * https://oeis.org/A399523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of powerful numbers ≤ the n‑th perfect power.
 *            Equivalent: a(n) = A217038(A001597(n)) + 1
 * Range:     u8 n <= 165, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cbrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/int_cbrt.h"

/* integer square root (floor) using binary search */
static idx_t int_sqrt(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((__int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* comparison function for qsort on idx_t values */
static int cmp_idx(const void *a, const void *b) {
    const idx_t *x = (const idx_t *)a;
    const idx_t *y = (const idx_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* dynamic array helpers */
static void ensure_cap(void **ptr, size_t *cap, size_t needed, size_t elem_size) {
    if (*cap < needed) {
        size_t newcap = (*cap == 0) ? 8 : (*cap << 1);
        if (newcap < needed) newcap = needed;
        *ptr = realloc(*ptr, newcap * elem_size);
        *cap = newcap;
    }
}

/* ------------------------------------------------------------------ */
/*  data structures                                                 */
/* ------------------------------------------------------------------ */
static bool initialized = false;
static idx_t *perfect_powers = NULL;    /* sorted unique perfect powers   */
static idx_t *powerful_nums  = NULL;    /* sorted unique powerful numbers */
static idx_t *rank_in_powerful = NULL;  /* a(n) values, 0‑based index */
static size_t pp_len = 0, pn_len = 0;

/* maximum number of perfect powers to pre‑compute */
static const idx_t MAX_N = 20000;

/* ------------------------------------------------------------------ */
/*  precomputation routine                                          */
/* ------------------------------------------------------------------ */
static void init_data(void) {
    if (initialized) return;
    initialized = true;

    /* ----------------------------------------------- */
    /*  generate perfect powers up to (MAX_N+1)^2      */
    /* ----------------------------------------------- */
    idx_t limit = (MAX_N + 1) * (MAX_N + 1);    /* safe in 64‑bit              */
    size_t pp_cap = 0;
    idx_t *pp = NULL;
    pp_len = 0;

    /* 1 is a perfect power */
    ensure_cap((void **)&pp, &pp_cap, pp_len + 1, sizeof(idx_t));
    pp[pp_len++] = 1;

    idx_t max_base = int_sqrt(limit);
    for (idx_t base = 2; base <= max_base; ++base) {
        idx_t power = (idx_t)base * base;          /* base^2 */
        while (power <= limit) {
            ensure_cap((void **)&pp, &pp_cap, pp_len + 1, sizeof(idx_t));
            pp[pp_len++] = power;
            if (power > limit / base) break;
            power *= base;                        /* next power */
        }
    }
    qsort(pp, pp_len, sizeof(idx_t), cmp_idx);

    /* deduplicate */
    size_t uniq = 0;
    for (size_t i = 0; i < pp_len; ++i) {
        if (i == 0 || pp[i] != pp[i - 1]) {
            pp[uniq++] = pp[i];
        }
    }
    pp_len = uniq;
    perfect_powers = pp;

    /* ----------------------------------------------- */
    /*  generate powerful numbers up to the same limit */
    /* ----------------------------------------------- */
    size_t pn_cap = 0;
    idx_t *pn = NULL;
    pn_len = 0;

    idx_t max_a = int_sqrt(limit);
    idx_t max_b = int_cbrt(limit);

    /* precompute squarefree flag for b up to max_b */
    bool *squarefree = (bool *)calloc(max_b + 1, sizeof(bool));
    for (size_t i = 0; i <= max_b; ++i) squarefree[i] = true;
    squarefree[0] = false;
    for (size_t i = 2; i * i <= max_b; ++i) {
        size_t square = i * i;
        for (size_t j = square; j <= max_b; j += square)
            squarefree[j] = false;
    }

    for (idx_t a = 1; a <= max_a; ++a) {
        idx_t a2 = a * a;
        idx_t max_b_for_a = int_cbrt(limit / a2);
        for (idx_t b = 1; b <= max_b_for_a; ++b) {
            if (!squarefree[b]) continue;               /* need b squarefree   */
            idx_t b3 = b * b * b;                       /* b^3                */
            idx_t val = a2 * b3;
            if (val > limit) break;
            ensure_cap((void **)&pn, &pn_cap, pn_len + 1, sizeof(idx_t));
            pn[pn_len++] = val;
        }
    }
    free(squarefree);

    qsort(pn, pn_len, sizeof(idx_t), cmp_idx);

    /* deduplicate powerful numbers (just in case) */
    uniq = 0;
    for (size_t i = 0; i < pn_len; ++i) {
        if (i == 0 || pn[i] != pn[i - 1]) pn[uniq++] = pn[i];
    }
    pn_len = uniq;
    powerful_nums = pn;

    /* ----------------------------------------------- */
    /*  compute rank array: number of powerful <= each pp */
    /* ----------------------------------------------- */
    rank_in_powerful = (idx_t *)malloc(pp_len * sizeof(idx_t));
    size_t pn_index = 0;
    for (size_t i = 0; i < pp_len; ++i) {
        idx_t val = perfect_powers[i];
        while (pn_index < pn_len && powerful_nums[pn_index] <= val)
            ++pn_index;
        rank_in_powerful[i] = pn_index;                /* 0‑based count */
    }
}

/* ------------------------------------------------------------------ */
/*  public function                                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    init_data();

    if (n == 0 || n > (idx_t)pp_len) return (seq_t)0;
    return (seq_t)rank_in_powerful[n - 1];
}
