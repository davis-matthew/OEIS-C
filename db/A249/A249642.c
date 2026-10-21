/*
 * A249642  Number of strings of length n over a 9-letter alphabet that begin
 *          with a nontrivial palindrome.
 * https://oeis.org/A249642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A249642
   Recurrence:  a(0)=0, a(1)=0,
                a(n+1) = 9*a(n) + 9^ceil((n+1)/2) - a(ceil((n+1)/2))
   (offset 0) */
#include <stdlib.h>

static seq_t *a_cache    = NULL;   /* cache of a(0)…a(k) */
static size_t a_cap      = 0;      /* allocated size of a_cache   */
static idx_t  a_up_to   = 1;       /* highest index already computed (≥1) */

static seq_t *pow9_cache = NULL;   /* cache of 9^0…9^k   */
static size_t pow_cap    = 0;      /* allocated capacity of pow9_cache   */
static idx_t  pow_up_to  = 0;      /* highest exponent already computed (≥0) */

/* initialise a_cache[0] and a_cache[1] if necessary */
static void init_a_cache(void)
{
    if (a_cap == 0) {
        a_cap = 2;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = 0;
        a_cache[1] = 0;
    }
}

/* initialise pow9_cache[0] if necessary */
static void init_pow9_cache(void)
{
    if (pow_cap == 0) {
        pow_cap = 1;
        pow9_cache = (seq_t *)malloc(pow_cap * sizeof(seq_t));
        pow9_cache[0] = 1;  /* 9^0 = 1 */
        pow_up_to = 0;
    }
}

/* enlarge a_cache to hold at least indices 0…n */
static void ensure_a_cache(idx_t n)
{
    if ((size_t)n < a_cap) return;
    size_t new_cap = (a_cap == 0) ? 2 : a_cap * 2;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

/* enlarge pow9_cache to hold exponents 0…k */
static void ensure_pow9_cache(idx_t k)
{
    if (k <= pow_up_to) return;          /* already computed */
    /* ensure capacity for indices 0…k */
    if ((size_t)(k + 1) > pow_cap) {
        size_t new_cap = (pow_cap == 0) ? 1 : pow_cap;
        while ((size_t)(k + 1) > new_cap) new_cap <<= 1;
        pow9_cache = (seq_t *)realloc(pow9_cache, new_cap * sizeof(seq_t));
        pow_cap = new_cap;
    }
    for (idx_t i = pow_up_to + 1; i <= k; ++i) {
        pow9_cache[i] = pow9_cache[i - 1] * (seq_t)9;
    }
    pow_up_to = k;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    init_a_cache();
    init_pow9_cache();

    ensure_a_cache(n);

    /* compute missing a(i) values */
    for (idx_t i = a_up_to + 1; i <= n; ++i) {
        idx_t half = (i + 1) / 2;          /* ceil(i/2) */
        ensure_pow9_cache(half);

        seq_t term = pow9_cache[half];
        term += a_cache[i - 1] * (seq_t)9;
        term -= a_cache[half];

        a_cache[i] = term;
    }

    if (n > a_up_to) a_up_to = n;
    return a_cache[n];
}
