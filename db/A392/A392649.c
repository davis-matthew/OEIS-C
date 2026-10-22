/*
 * A392649  Numbers that can be written as the sum of a subset of their
 *          sum-irreducible nontrivial divisors.
 * https://oeis.org/A392649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A392649: Numbers that sum to themselves via a subset of their
   sum‑irreducible nontrivial divisors (solid numbers).
   Computed by testing each integer in order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* --- Constants ----------------------------------------------- */
#define MAX_DIVISORS      2048          /* upper bound on divisors list */
#define SIEVE_LIMIT       5000000u      /* pre‑compute primes up to this limit */

/* --- Static tables ------------------------------------------- */
static uint32_t *spf                = NULL;   /* smallest prime factor */
static int8_t  *sum_irred_cache     = NULL;   /* -1 unknown, 0 false, 1 true */
static seq_t   *solid_cache         = NULL;   /* found solid numbers  */
static size_t  solid_cap            = 0;
static size_t  solid_size           = 0;
static uint32_t last_checked_k      = 2;

/* --- Utility: grow dynamic array of solids ------------------- */
static void ensure_solids_cap(size_t needed)
{
    if (solid_cap >= needed) return;
    size_t cap = solid_cap ? solid_cap * 2 : 16;
    if (cap < needed) cap = needed * 2;
    solid_cache = (seq_t *)realloc(solid_cache, cap * sizeof(seq_t));
    solid_cap = cap;
}

/* --- Sieve of Eratosthenes: smallest prime factor ------------ */
static void init_tables(void)
{
    if (spf != NULL) return;                    /* already built */

    spf = (uint32_t *)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    sum_irred_cache = (int8_t *)malloc((SIEVE_LIMIT + 1) * sizeof(int8_t));
    if (!spf || !sum_irred_cache) exit(1);

    for (uint32_t i = 0; i <= SIEVE_LIMIT; ++i) {
        spf[i] = i;
        sum_irred_cache[i] = -1;
    }

    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == i) {                     /* i is prime */
            for (uint64_t j = (uint64_t)i * i; j <= SIEVE_LIMIT; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
}

/* --- Factorisation using pre‑computed SPF --------------------- */
static void factor_uint32(uint32_t n, uint32_t *pf, int *pe, size_t *cnt)
{
    *cnt = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        pf[(*cnt)] = p;
        pe[(*cnt)] = e;
        ++(*cnt);
    }
}

/* --- Generate all divisors of n (including 1 and n) ----------- */
static void gen_divisors_rec(const uint32_t *pf, const int *pe,
                             size_t fcnt, size_t idx, uint32_t cur,
                             uint32_t *out, size_t *outcnt)
{
    if (idx == fcnt) {
        out[(*outcnt)] = cur;
        ++(*outcnt);
        return;
    }
    uint32_t p = pf[idx];
    uint32_t mul = 1;
    for (int e = 0; e <= pe[idx]; ++e) {
        gen_divisors_rec(pf, pe, fcnt, idx + 1, cur * mul, out, outcnt);
        mul *= p;
    }
}

static void get_divisors(uint32_t n, uint32_t *out, size_t *cnt)
{
    uint32_t pf[MAX_DIVISORS / 2];
    int pe[MAX_DIVISORS / 2];
    size_t fcnt;
    factor_uint32(n, pf, pe, &fcnt);
    *cnt = 0;
    gen_divisors_rec(pf, pe, fcnt, 0, 1, out, cnt);
}

/* --- Descending quick sort for small arrays ------------------- */
static void sort_desc(uint32_t *arr, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            if (arr[j] > arr[i]) {
                uint32_t t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
        }
    }
}

/* --- Subset‑sum test with pruning (recursive) ----------------- */
static bool subset_rec(const uint32_t *arr, size_t count,
                       size_t idx, seq_t remain,
                       const seq_t *suff)
{
    if (remain == 0) return true;
    if (idx == count) return false;
    if (remain > suff[idx]) return false;           /* cannot reach target */

    /* include current divisor */
    if ((seq_t)arr[idx] <= remain &&
        subset_rec(arr, count, idx + 1, remain - arr[idx], suff))
        return true;

    /* skip current divisor */
    return subset_rec(arr, count, idx + 1, remain, suff);
}

static bool subset_sum(const uint32_t *arr, size_t count, seq_t target)
{
    if (count == 0) return target == 0;
    seq_t suff[MAX_DIVISORS];
    seq_t accum = 0;
    for (size_t i = count; i-- > 0; ) {
        accum += (seq_t)arr[i];
        suff[i] = accum;
    }
    return subset_rec(arr, count, 0, target, suff);
}

/* --- Determine if n is sum‑irreducible ------------------------ */
static bool is_sum_irreducible(uint32_t n)
{
    if (n <= 2) return true;                      /* prime or 2 */
    int8_t cached = sum_irred_cache[n];
    if (cached != -1) return cached;

    uint32_t all_divs[MAX_DIVISORS];
    size_t   all_cnt;
    get_divisors(n, all_divs, &all_cnt);

    /* remove 1 and n itself */
    size_t dcnt = 0;
    for (size_t i = 0; i < all_cnt; ++i) {
        uint32_t d = all_divs[i];
        if (d > 1 && d < n) all_divs[dcnt++] = d;
    }

    bool irreducible = true;
    if (dcnt > 0) {
        sort_desc(all_divs, dcnt);
        if (subset_sum(all_divs, dcnt, (seq_t)n))
            irreducible = false;
    }
    sum_irred_cache[n] = irreducible ? 1 : 0;
    return irreducible;
}

/* --- Determine if k is solid --------------------------------- */
static bool is_solid(uint32_t k)
{
    uint32_t all_divs[MAX_DIVISORS];
    size_t   all_cnt;
    get_divisors(k, all_divs, &all_cnt);

    uint32_t irreds[MAX_DIVISORS];
    size_t   ircnt = 0;
    for (size_t i = 0; i < all_cnt; ++i) {
        uint32_t d = all_divs[i];
        if (d > 1 && d < k && is_sum_irreducible(d))
            irreds[ircnt++] = d;
    }
    if (ircnt == 0) return false;

    sort_desc(irreds, ircnt);
    return subset_sum(irreds, ircnt, (seq_t)k);
}

/* --- Main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    init_tables();                     /* ensure SPF and caches are ready */

    /* generate solid numbers until we have at least n terms */
    while (solid_size < n) {
        if (last_checked_k > SIEVE_LIMIT) {
            /* For n up to 10 000 the limit above is sufficient. */
            break;
        }
        if (is_solid(last_checked_k)) {
            ensure_solids_cap(solid_size + 1);
            solid_cache[solid_size] = (seq_t)last_checked_k;
            ++solid_size;
        }
        ++last_checked_k;
    }

    return solid_cache[n - 1];
}
