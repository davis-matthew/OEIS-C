/*
 * A353868  Numbers k such that the Carmichael function A002322(k) divides
 *          Dedekind psi A001615(k).
 * https://oeis.org/A353868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353868
   a(n) = the n-th positive integer k such that the Carmichael function
          λ(k) divides the Dedekind ψ(k). */

/* This implementation tests each integer sequentially and stores
   those that satisfy the condition.  It uses only the operations
   allowed for seq_t and does not depend on any external helper
   functions. */

#include <stdint.h>
#include <stdlib.h>

/* ----------  local helper functions  ---------- */

static inline seq_t seq_pow(seq_t base, unsigned int exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---------------------------------------------------------- */
/* check whether a positive integer k satisfies λ(k) | ψ(k)   */
static int good(uint64_t k)
{
    if (k == 1) return 1;

    uint64_t n = k;                 /* remaining factor to be processed */
    seq_t lambda_lcm = 1;           /* lcm of λ(p^e) for processed primes */
    seq_t psi = 1;                  /* Dedekind ψ(k) */

    /* handle factor 2 separately */
    if (n % 2 == 0)
    {
        int e = 0;
        while (n % 2 == 0) { n /= 2; ++e; }

        seq_t p_pow = (e > 1) ? seq_pow((seq_t)2, e - 1) : 1;   /* 2^(e-1) */
        psi = psi * p_pow;
        psi = psi * (seq_t)3;                                 /* (2+1) */

        seq_t lambda_p;
        if (e == 1) lambda_p = 1;
        else if (e == 2) lambda_p = 2;
        else lambda_p = seq_pow((seq_t)2, e - 2);             /* 2^(e-2) */

        seq_t g = seq_gcd(lambda_lcm, lambda_p);
        lambda_lcm = (lambda_lcm / g) * lambda_p;
    }

    /* loop over odd primes */
    for (uint64_t p = 3; p * p <= n; p += 2)
    {
        if (n % p == 0)
        {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }

            seq_t p_pow = seq_pow((seq_t)p, e - 1);           /* p^(e-1) */
            psi = psi * p_pow;
            psi = psi * (seq_t)(p + 1);                      /* (p+1) */

            seq_t lambda_p = p_pow * (seq_t)(p - 1);         /* λ(p^e) */
            seq_t g = seq_gcd(lambda_lcm, lambda_p);
            lambda_lcm = (lambda_lcm / g) * lambda_p;
        }
    }

    /* remaining prime factor (if any) */
    if (n > 1)
    {
        uint64_t p = n;                                      /* exponent 1 */
        seq_t p_pow = 1;                                      /* p^(1-1) */
        psi = psi * p_pow;
        psi = psi * (seq_t)(p + 1);

        seq_t lambda_p = (seq_t)(p - 1);                       /* λ(p) */
        seq_t g = seq_gcd(lambda_lcm, lambda_p);
        lambda_lcm = (lambda_lcm / g) * lambda_p;
    }

    return (psi % lambda_lcm) == 0;
}

/* ----------  sequence storage  ---------- */

static seq_t *seq_arr = NULL;   /* dynamic array of found terms */
static size_t seq_cap = 0;      /* capacity of seq_arr */
static size_t seq_len = 0;      /* number of terms stored          */
static uint64_t next_candidate = 1; /* integer to test next */

/* grow seq_arr when needed */
static void grow_seq_arr(size_t need)
{
    if (seq_cap < need)
    {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap < need) new_cap *= 2;
        seq_arr = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* ---------------------------------------------------------- */
/* required exported function                                 */
seq_t a(idx_t n)
{
    /* enforce 1‑based indexing */
    while (seq_len < (size_t)n)
    {
        if (good(next_candidate))
        {
            grow_seq_arr(seq_len + 1);
            seq_arr[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return seq_arr[(size_t)n - 1];
}
