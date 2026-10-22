/*
 * A375768  The maximum exponent in the prime factorization of the numbers
 *          whose maximum exponent in their prime factorization is a Fibonacci
 *          number.
 * https://oeis.org/A375768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A375768
   a(n) = maximum exponent in the prime factorization of the n‑th
   integer whose maximum exponent is a Fibonacci number.
   The sequence consists only of Fibonacci numbers. */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <new>
#endif

/* ------------------------------------------------------------------ */
/* Fibonacci flags for exponents up to 127 (enough for the intended range). */
static bool fib_flag[128] = { false };

/* ------------------------------------------------------------------ */
/* Sequence storage (values of a(n)). */
static seq_t *seq_vals = NULL;   /* dynamic array of sequence values      */
static size_t seq_len = 0;       /* number of terms currently stored      */
static size_t seq_cap = 0;       /* allocated capacity for seq_vals       */

/* ------------------------------------------------------------------ */
/* Current integer being examined in the sieve. */
static idx_t next_check = 1;

/* ------------------------------------------------------------------ */
/* Smallest prime factor array. */
static uint32_t *spf = NULL;     /* spf[i] = smallest prime factor of i  */
static idx_t spf_limit = 0;      /* highest index for which spf is valid  */

/* ------------------------------------------------------------------ */
/* Block size for spf expansion; a few thousand is ample for n≈10000. */
#define SPF_BLOCK 1000

/* ------------------------------------------------------------------ */
/* Initialise the Fibonacci flag table. */
static void init_fib_flags(void)
{
    if (fib_flag[0]) return;          /* already initialized */

    int a = 0, b = 1;
    fib_flag[a] = true;
    fib_flag[b] = true;

    while (b < 127) {
        int c = a + b;
        a = b;
        b = c;
        if (b < 127) fib_flag[b] = true;
    }
}

/* ------------------------------------------------------------------ */
/* Resize the spf array to cover all indices up to 'limit' and compute
   the smallest prime factor for each. */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit) return;   /* already covers needed range */

    /* Allocate or grow the array. */
    uint32_t *new_spf = (uint32_t *)realloc(spf, (size_t)(limit + 1) * sizeof(uint32_t));
    if (!new_spf) abort();            /* memory allocation failure   */
    spf = new_spf;
    spf_limit = limit;

    /* Initialise: spf[i] = i. */
    for (idx_t i = 2; i <= limit; ++i) spf[i] = (uint32_t)i;

    /* Eratosthenes sieve to compute smallest prime factor. */
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (spf[i] == i) {            /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Return the maximum exponent in the prime factorization of x. */
static seq_t max_exponent(idx_t x)
{
    if (x <= 1) return 0;            /* 1 has max exponent 0 */

    seq_t max_e = 0;
    while (x > 1) {
        idx_t p = spf[x];             /* spf must be valid for x */
        idx_t cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }
        if (cnt > max_e) max_e = (seq_t)cnt;
    }
    return max_e;
}

/* ------------------------------------------------------------------ */
/* Allocate (or reallocate) seq_vals to at least 'new_cap' elements.
   Works for both C (using malloc/realloc) and C++ (using new[]). */
static void ensure_seq_capacity(size_t new_cap)
{
    if (seq_cap >= new_cap) return;

#ifdef __cplusplus
    seq_t *new_vals = new seq_t[new_cap];
    if (seq_vals) {
        for (size_t i = 0; i < seq_len; ++i) new_vals[i] = seq_vals[i];
        delete[] seq_vals;
    }
    seq_vals = new_vals;
#else
    seq_t *new_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!new_vals) abort();
    seq_vals = new_vals;
#endif
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Extend the sequence array until it contains at least 'need' terms. */
static void extend_sequence(size_t need)
{
    /* Allocate more space if necessary. */
    if (seq_cap < need) ensure_seq_capacity(need * 2);

    /* Generate terms until we have enough. */
    while (seq_len < need) {
        /* Ensure spf covers the current integer. */
        if (next_check > spf_limit) {
            idx_t new_limit = next_check + SPF_BLOCK;
            ensure_spf(new_limit);
        }

        seq_t val = max_exponent(next_check);
        if (val < 128 && fib_flag[(int)val]) {
            seq_vals[seq_len++] = val;   /* store only Fibonacci exponents */
        }
        ++next_check;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for out‑of‑range */

    /* First initialisation. */
    init_fib_flags();

    /* Extend the sequence until it has at least n terms. */
    if (seq_len < n) {
        extend_sequence((size_t)n);
    }

    return seq_vals[n - 1];
}
