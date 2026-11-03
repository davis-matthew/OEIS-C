/*
 * A256015  Triangle read by rows: n-th row contains all distinct primes which
 *          are representable as the sum of some subset of the set of first n
 *          primes.
 * https://oeis.org/A256015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th term of the triangle where the r‑th row *
 *            consists of all primes that can be expressed as the sum of a
 *            subset * of the first r primes. The sequence is obtained by
 *            reading the * triangle row by row
 * Range:     u8 n <= 310, u16 n <= 1606, u32 n <= 1606, u64 n <= 1606,
 *            u128 n <= 1606, big any n
 * Validated: standard, pass, 85 terms through n = 1584 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----------  static state used by the function  ---------- */

static idx_t   current_row = 0;      /* number of primes processed so far (0 at start) */
static idx_t   max_sum     = 0;      /* maximum sum reachable in current reach array */

static bool *  reach       = NULL;   /* bitset of reachable sums: 1 = reachable */
static size_t  reach_cap   = 0;      /* allocated capacity for reach (number of sums) */

/* storage of rows that have been generated */
static seq_t **  row_ptrs   = NULL;  /* array of pointers to prime arrays per row */
static size_t  *  row_len    = NULL;  /* length of each row */
static idx_t  *   cumul      = NULL;  /* cumulative term count up to each row */
static size_t    rows_cap   = 0;      /* capacity of row arrays */
static size_t    row_cnt    = 0;      /* number of rows stored */

/* ----------  helper functions  ---------- */

/* grow the reach array to hold at least new_cap elements (sums) */
static void grow_reach(idx_t new_cap)
{
    if (new_cap <= reach_cap) return;
    reach = (bool *)realloc(reach, new_cap * sizeof(bool));
    memset(reach + reach_cap, 0, (new_cap - reach_cap) * sizeof(bool));
    reach_cap = new_cap;
}

/* grow the row storage arrays to hold at least needed rows */
static void grow_rows(size_t needed)
{
    if (needed <= rows_cap) return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    row_ptrs = (seq_t **)realloc(row_ptrs, new_cap * sizeof(seq_t *));
    row_len  = (size_t *)realloc(row_len,  new_cap * sizeof(size_t));
    cumul    = (idx_t *)realloc(cumul,  new_cap * sizeof(idx_t));
    rows_cap = new_cap;
}

/* generate the next row (i.e., add the next prime to the subset sums) */
static void generate_next_row(void)
{
    ++current_row;                                 /* move to next prime index (1‑based) */
    seq_t prime = (seq_t)get_prime(current_row);   /* value of the prime */
    idx_t prime_idx = (idx_t)prime;                /* shift amount – fits in idx_t for our use */

    /* extend reach array to accommodate new sums */
    idx_t new_max = max_sum + prime_idx;
    grow_reach(new_max + 1);

    /* add the new prime to existing sums – iterate downwards to avoid reuse of
     * the same prime more than once in a single row */
    for (idx_t i = new_max; i >= 1; --i) {
        if (i >= prime_idx && reach[i - prime_idx]) reach[i] = true;
    }
    /* reach[0] remains true (empty subset) */
    max_sum = new_max;

    /* collect the primes among reachable sums */
    size_t primes_cap = 16;
    seq_t *primes_cur = mem_new(seq_t, primes_cap);
    size_t primes_len = 0;

    for (idx_t s = 2; s <= max_sum; ++s) {
        if (reach[s] && is_prime_big((seq_t)s)) {
            if (primes_len == primes_cap) {
                primes_cap <<= 1;
                primes_cur = mem_resize(seq_t, primes_cur, primes_cap);
            }
            primes_cur[primes_len++] = (seq_t)s;
        }
    }

    /* store the row for future queries */
    grow_rows(row_cnt + 1);
    row_ptrs[row_cnt] = primes_cur;
    row_len[row_cnt]  = primes_len;
    cumul[row_cnt]    = (row_cnt == 0 ? (idx_t)primes_len
                                      : cumul[row_cnt - 1] + (idx_t)primes_len);
    ++row_cnt;
}

/* ----------  public function required by the harness  ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, return 0 as safe default */

    /* initialise reach if this is the first call */
    if (reach == NULL) {
        reach_cap = 1;
        reach = (bool *)malloc(reach_cap * sizeof(bool));
        reach[0] = true;          /* sum 0 is always reachable */
    }

    /* generate rows until the cumulative count reaches or exceeds n */
    while (row_cnt == 0 || cumul[row_cnt - 1] < n) {
        generate_next_row();
    }

    /* locate the row containing the n‑th term */
    size_t r = 0;
    while (r < row_cnt && cumul[r] < n) ++r;

    idx_t before = (r == 0) ? 0 : cumul[r - 1];
    size_t pos = (size_t)(n - before - 1);          /* 0‑based index within the row */
    return row_ptrs[r][pos];
}
