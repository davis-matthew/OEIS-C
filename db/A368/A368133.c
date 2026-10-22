/*
 * A368133  a(1,2,3) = 1,2,3; let j = a(n-1), M(n) = Product_{i = 1..n-2} { p
 *          a distinct prime: p | a(i), gcd(p, j) = 1 }. For n > 3, a(n) is
 *          the least novel multiple of M(n) if M(n) > 1; otherwise a(n) is
 *          the least novel multiple of A053669(j), the smallest prime which
 *          does not divide j.
 * https://oeis.org/A368133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=3. For n>3 let j=a(n-1). M(n) = product of
 *            all distinct primes p that divide some a(i) for i≤n-2 but do NOT
 *            divide j. a(n) = if M(n) > 1: the least multiple of M(n) that is
 *            NOT in the sequence so far, else: the least multiple of the
 *            smallest prime that does NOT divide j
 * Range:     u8 n <= 33, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool is_prime_small(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if ((x & 1) == 0) return false;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* ----- Dynamic storage for the sequence ---------------------------------- */
static seq_t *seq = NULL;      /* array of computed terms */
static size_t seq_len = 0;     /* number of terms stored */
static size_t seq_cap = 0;     /* array capacity */

/* ----- Dynamic storage for all distinct primes seen so far ----------------- */
static seq_t *primes = NULL;   /* array of all primes that divide any term */
static size_t primes_len = 0;  /* number of primes stored */
static size_t primes_cap = 0;  /* array capacity */

/* Ensure seq has capacity at least 'needed'   (C++ cast included) */
static void ensure_seq_cap(size_t needed)
{
    if (needed > seq_cap) {
        size_t ncap = (seq_cap == 0) ? 8 : seq_cap << 1;
        if (ncap < needed) ncap = needed;
        seq = (seq_t *)realloc(seq, ncap * sizeof(seq_t));
        seq_cap = ncap;
    }
}

/* Ensure primes array has capacity at least 'needed' */
static void ensure_primes_cap(size_t needed)
{
    if (needed > primes_cap) {
        size_t ncap = (primes_cap == 0) ? 8 : primes_cap << 1;
        if (ncap < needed) ncap = needed;
        primes = (seq_t *)realloc(primes, ncap * sizeof(seq_t));
        primes_cap = ncap;
    }
}

/* Add prime to list if it is not already present. */
static void add_prime(seq_t p)
{
    for (size_t i = 0; i < primes_len; ++i)
        if (primes[i] == p)
            return;           /* already recorded */
    ensure_primes_cap(primes_len + 1);
    primes[primes_len++] = p;
}

/* Factorise 'term' and record all prime factors. */
static void factorize_term(seq_t term)
{
    seq_t x = term;
    /* Handle 2 separately */
    if ((x & 1) == 0) {
        add_prime((seq_t)2);
        while ((x & 1) == 0) x >>= 1;
    }
    /* Trial division by odd numbers */
    for (seq_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) {
            add_prime(d);
            while (x % d == 0) x /= d;
        }
    }
    if (x > (seq_t)1) add_prime(x);  /* remaining prime factor */
}

/* Linear search for existence of 'value' in seq[0..seq_len-1]. */
static bool exists_in_seq(seq_t value)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq[i] == value)
            return true;
    return false;
}

/* Return the least multiple of 'base' that is not yet in the sequence. */
static seq_t next_novel_multiple(seq_t base)
{
    seq_t cand = base;
    while (exists_in_seq(cand))
        cand += base;
    return cand;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety for out-of-range argument */

    /* --- Initialise the first three terms if necessary --- */
    if (seq_len == 0) {
        ensure_seq_cap(3);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        seq[2] = (seq_t)3;
        seq_len = 3;
        /* Record prime factors of the first three terms */
        factorize_term(seq[0]);
        factorize_term(seq[1]);
        factorize_term(seq[2]);
    }

    /* --- Extend the sequence until it contains n terms --- */
    while (seq_len < (size_t)n) {
        seq_t j = seq[seq_len - 1];      /* a(n-1) for the next term */

        /* Compute M(n) as product of primes that divide some earlier term
           but do NOT divide j. */
        seq_t M = (seq_t)1;
        for (size_t i = 0; i < primes_len; ++i) {
            seq_t p = primes[i];
            if (j % p != (seq_t)0)
                M *= p;
        }

        seq_t base;
        if (M > (seq_t)1)
            base = M;
        else {
            /* Find smallest prime not dividing j */
            uint64_t candidate = 2;
            for (;;) {
                seq_t cand = (seq_t)candidate;
                if (j % cand == (seq_t)0) { /* divides j -> skip */
                    ++candidate;
                    continue;
                }
                if (is_prime_small(candidate)) {
                    base = cand;
                    break;
                }
                ++candidate;
            }
        }

        seq_t term = next_novel_multiple(base);

        /* Append the new term */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len++] = term;

        /* Record its prime factors for future M(n) computations */
        factorize_term(term);
    }

    return seq[n - 1];
}
