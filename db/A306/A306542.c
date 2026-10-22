/*
 * A306542  The least prime q such that Kronecker(D/q) >= 0 where D runs
 *          through all negative fundamental discriminants (-A003657).
 * https://oeis.org/A306542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A306542:  a(n) = the least prime q such that Kronecker(D/q) ≥ 0,
    where D = -A003657(n)  (fundamental discriminant of an
    imaginary quadratic field).  The code generates the
    corresponding positive discriminants d = -D, tests them
    for the fundamental discriminant condition, and then
    searches for the smallest prime whose Kronecker symbol with
    D is non‑negative.  It uses a simple sieve to pre‑compute
    primes up to 1 000 000 and trial‑division to test square‑free
    numbers.  All intermediate arithmetic is performed with
    the types seq_t and idx_t supplied by the build.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/jacobi_symbol.h"

/* ---------- prime sieve (once) ---------- */
static idx_t *primes = NULL;
static size_t prime_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    const idx_t LIMIT = 1000000ull;          /* sufficient for all required primes   */
    bool *comp = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!comp) abort();

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (!comp[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                comp[j] = true;
        }
    }

    size_t c = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!comp[i]) ++c;

    primes = (idx_t*)malloc(c * sizeof(idx_t));
    if (!primes) abort();

    size_t k = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!comp[i]) primes[k++] = i;

    prime_cnt = c;
    free(comp);
    primes_initialized = true;
}

/* ---------- square‑free test (trial division) ---------- */
static bool is_squarefree(idx_t n)
{
    if (n <= 1) return true;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) {
            n /= p;
            if (n % p == 0) return false;
        }
    }
    return true;
}

/* ---------- fundamental discriminants list ---------- */
static idx_t *fundamental = NULL;
static size_t fund_size = 0, fund_cap = 0;
static idx_t next_candidate = 3;   /* start with d = 3 (|D|=3) */

static void add_fundamental(idx_t d)
{
    if (fund_size == fund_cap) {
        size_t new_cap = fund_cap ? fund_cap * 2 : 16;
        idx_t *tmp = (idx_t*)realloc(fundamental, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        fundamental = tmp;
        fund_cap = new_cap;
    }
    fundamental[fund_size++] = d;
}

static void ensure_fundamental(idx_t n)
{
    if (!primes_initialized) init_primes();

    while (fund_size < n) {
        idx_t d = next_candidate++;
        if ((d % 4 == 3) && is_squarefree(d)) {                /* odd case: D≡1 (mod 4) */
            add_fundamental(d);
        } else if (d % 4 == 0) {                              /* even case: D≡0 (mod 4) */
            idx_t m = d / 4;
            if (is_squarefree(m) && (m % 4 == 1 || m % 4 == 2))
                add_fundamental(d);
        }
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n==0 by OEIS specification */

    ensure_fundamental(n);
    idx_t d = fundamental[n - 1];   /* |D| */

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        int sign;

        if (p == 2) {
            if (d & 1ULL) {                     /* D odd */
                uint64_t r = d % 8;
                sign = (r == 1 || r == 7) ? 1 : -1;
            } else {                            /* D even -> ramified */
                sign = 0;
            }
        } else {
            if (d % p == 0) {                   /* divides D: ramified */
                sign = 0;
            } else {
                uint64_t a = (p - (d % p)) % p; /* compute (-d) mod p */
                sign = jacobi_symbol(a, p);
            }
        }

        if (sign >= 0)                    /* splits or ramifies */
            return (seq_t)p;
    }

    /* If the prime list is exhausted (extremely unlikely for n≤10000),
       extend the sieve a bit and retry. */
    /* This part is rarely exercised and keeps the function robust. */
    {
        const idx_t NEW_LIMIT = 2000000ull;   /* extend to 2 M if needed */
        bool *comp = (bool*)calloc(NEW_LIMIT + 1, sizeof(bool));
        if (!comp) abort();

        for (idx_t i = 2; i * i <= NEW_LIMIT; ++i)
            if (!comp[i])
                for (idx_t j = i * i; j <= NEW_LIMIT; j += i)
                    comp[j] = true;

        size_t c = 0;
        for (idx_t i = 2; i <= NEW_LIMIT; ++i)
            if (!comp[i]) ++c;

        if (prime_cnt + c > fund_cap) {
            size_t new_cap = fund_cap ? fund_cap * 2 : 16;
            while (new_cap < prime_cnt + c) new_cap *= 2;
            idx_t *tmp = (idx_t*)realloc(primes, new_cap * sizeof(idx_t));
            if (!tmp) abort();
            primes = tmp;
            fund_cap = new_cap;
        }

        size_t k = 0;
        for (idx_t i = 2; i <= NEW_LIMIT; ++i)
            if (!comp[i]) primes[prime_cnt + k++] = i;

        prime_cnt += k;
        free(comp);
    }

    /* Retry with the extended prime list. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        int sign;
        if (p == 2) {
            if (d & 1ULL) {
                uint64_t r = d % 8;
                sign = (r == 1 || r == 7) ? 1 : -1;
            } else
                sign = 0;
        } else {
            if (d % p == 0)
                sign = 0;
            else {
                uint64_t a = (p - (d % p)) % p;
                sign = jacobi_symbol(a, p);
            }
        }
        if (sign >= 0) return (seq_t)p;
    }

    /* Should never reach here; return 0 as a safety fallback. */
    return (seq_t)0;
}
