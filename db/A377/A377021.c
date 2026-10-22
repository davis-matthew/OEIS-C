/*
 * A377021  Numbers whose prime factorization has exponents that are all sums
 *          of distinct factorials (A059590, where 0! and 1! are not
 *          considered distinct).
 * https://oeis.org/A377021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 240, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A377021: Numbers whose prime‐factor exponents are sums of
   distinct factorials (0! and 1! not considered distinct).  The code
   generates terms by simple trial division and filters by the
   allowed exponent set. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------
   Precomputed set of exponents that are sums of distinct factorials
   (factorials 1!,2!,3!,4!,5!,…).  We need only a small range because
   for n ≤ 10 000 the terms are below 20 000, thus prime exponents
   never exceed 14.  The table is still generous.
   ------------------------------------------------------------------ */
#define MAX_EXP 200
static bool allowed_exp[MAX_EXP + 1];
static bool allowed_init = false;

static void init_allowed(void)
{
    if (allowed_init) return;
    allowed_init = true;
    for (int i = 0; i <= MAX_EXP; ++i) allowed_exp[i] = false;

    /* factorials: 0!=1, 1!=1, 2!=2, 3!=6, 4!=24, 5!=120 … */
    const int fact[] = {1, 2, 6, 24, 120};
    const int nf = sizeof(fact) / sizeof(fact[0]);

    /* enumerate all subset sums (distinct factorials) */
    for (int mask = 0; mask < (1 << nf); ++mask) {
        int s = 0;
        for (int j = 0; j < nf; ++j)
            if (mask & (1 << j)) s += fact[j];
        if (s <= MAX_EXP) allowed_exp[s] = true;
    }
    allowed_exp[0] = true;         /* empty sum – exponent 0 */
}

/* ------------------------------------------------------------------
   Factorise n (≤ 20 000 for the required n) by trial division.
   exps stores the exponents; cnt is updated with the number of
   distinct prime factors.
   ------------------------------------------------------------------ */
static size_t factor_exponents(uint64_t n, uint8_t *exps)
{
    size_t cnt = 0;
    if (n <= 1) return cnt;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint8_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            exps[cnt++] = e;
        }
    }
    if (n > 1) exps[cnt++] = 1;    /* remaining prime factor */
    return cnt;
}

/* ------------------------------------------------------------------
   Return true iff all prime exponents are allowed.
   ------------------------------------------------------------------ */
static bool valid_exponents(uint64_t x)
{
    uint8_t exps[64];
    size_t c = factor_exponents(x, exps);
    for (size_t i = 0; i < c; ++i) {
        uint8_t e = exps[i];
        if (e > MAX_EXP || !allowed_exp[(int)e]) return false;
    }
    return true;
}

/* ------------------------------------------------------------------
   Cache for generated sequence terms.
   ------------------------------------------------------------------ */
static seq_t *seq_arr = NULL;
static size_t seq_cap = 0, seq_len = 0;
static uint64_t next_candidate = 1;

/* grow the static array, cast needed for arbitrary‑precision seq_t */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------
   Main exported function
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, but harmless */
    init_allowed();

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        uint64_t cur = next_candidate++;
        if (valid_exponents(cur)) {
            ensure_seq_cap(seq_len + 1);
            seq_arr[seq_len++] = (seq_t)cur;
        }
    }
    return seq_arr[n - 1];
}
