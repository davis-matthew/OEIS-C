/*
 * A397483  Numbers k such that tau(k - d) = tau(k) = tau(k + d), where 1 <= d
 *          <= k/4 is some divisor of k and tau(i) = A000005(i).
 * https://oeis.org/A397483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   compute the nth number k such that there exists a divisor d of k
 *            with 1 ≤ d ≤ k/4 for which tau(k‑d) = tau(k) = tau(k+d). Here
 *            tau is the divisor function tau(n) = A000005(n). The sequence is
 *            indexed from 1; a(1)=8, a(2)=34, …
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- configuration ---------- */
static const idx_t MAX_LIMIT = 10000000;   /* enough for n up to ~10 k */

/* ---------- global state ---------- */
static uint32_t *spf = NULL;      /* smallest prime factor table */
static idx_t    spf_limit = 0;     /* current spf table size */

static seq_t    *seq  = NULL;     /* generated sequence values */
static size_t   seq_len = 0;      /* number of stored terms */
static size_t   seq_cap = 0;      /* capacity of seq array */
static idx_t    current_k = 1;    /* next k to test */

/* ---------- helper functions ---------- */

/* initialise the spf table once */
static void init_spf(void)
{
    if (spf_limit >= MAX_LIMIT) return;

    spf = (uint32_t *)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    if (!spf) abort();          /* allocation failure */
    for (idx_t i = 0; i <= MAX_LIMIT; ++i) spf[i] = (uint32_t)i;

    for (idx_t i = 2; (idx_t)i * i <= MAX_LIMIT; ++i)
    {
        if (spf[i] == (uint32_t)i)               /* i is prime */
        {
            for (idx_t j = i * i; j <= MAX_LIMIT; j += i)
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;
        }
    }
    spf_limit = MAX_LIMIT;
}

/* compute tau(n) using the precomputed spf table */
static inline seq_t tau_count(seq_t n)
{
    seq_t result = 1;
    while (n > 1)
    {
        uint32_t p = spf[(idx_t)n];
        seq_t exp = 0;
        while (n % p == 0)
        {
            n /= p;
            ++exp;
        }
        result *= (exp + 1);
    }
    return result;
}

/* recursive divisor generator that checks the property */
static bool rec_check(size_t idx, seq_t cur,
                      const uint32_t primes[], const seq_t exps[], size_t cnt,
                      seq_t k, seq_t tau_k)
{
    if (idx == cnt)
    {
        seq_t d = cur;
        if (d >= 1 && d <= k / 4)
        {
            seq_t tau_left  = tau_count(k - d);
            seq_t tau_right = tau_count(k + d);
            if (tau_left == tau_k && tau_right == tau_k)
                return true;
        }
        return false;
    }

    /* iterate over exponent 0..exps[idx] */
    seq_t pow = 1;
    for (seq_t e = 0; e <= exps[idx]; ++e)
    {
        if (rec_check(idx + 1, cur * pow, primes, exps, cnt, k, tau_k))
            return true;
        pow *= primes[idx];
    }
    return false;
}

/* test whether k satisfies the defining property */
static bool has_property(seq_t k)
{
    seq_t tau_k = tau_count(k);

    /* factor k */
    uint32_t primes[32];
    seq_t    exps[32];
    size_t   count = 0;
    seq_t    temp = k;
    while (temp > 1)
    {
        uint32_t p = spf[(idx_t)temp];
        seq_t exp = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++exp;
        }
        primes[count] = p;
        exps[count]  = exp;
        ++count;
    }

    return rec_check(0, 1, primes, exps, count, k, tau_k);
}

/* ensure the sequence array has capacity at least needed */
static void seq_grow(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq) abort();
    seq_cap = new_cap;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but avoid UB */

    static bool initialized = false;
    if (!initialized)
    {
        init_spf();
        seq_grow(128);
        seq_len   = 0;
        seq_cap   = 128;
        current_k = 1;
        initialized = true;
    }

    /* generate terms until we have n of them */
    while (seq_len < (size_t)n)
    {
        if (current_k > MAX_LIMIT) abort(); /* safety guard */
        if (has_property(current_k))
        {
            seq_grow(seq_len + 1);
            seq[seq_len++] = current_k;
        }
        ++current_k;
    }

    return seq[n - 1];
}
