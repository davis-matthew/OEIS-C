/*
 * A347612  Semiprimes formed from single-digits primes only, each used at
 *          most once.
 * https://oeis.org/A347612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, fini, full, nonn
 * Formula:   a(n) = the nth smallest semiprime that can be formed by using a
 *            subset of the distinct single‑digit primes {2,3,5,7} each at
 *            most once, in any order. The sequence is finite (only 60 terms)
 * Range:     u8 none, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers – no external helpers needed. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Digits that may appear in the numbers. */
static const uint8_t digits[4] = {2, 3, 5, 7};

/* Simple check: is x a product of exactly two primes (counted with multiplicity)? */
static bool is_semiprime(uint64_t x)
{
    if (x < 4) return false;          /* 2*2 = 4 is the smallest semiprime */
    /* Find a non‑trivial divisor d. */
    uint64_t d = 0;
    for (uint64_t i = 2; i * i <= x; ++i) {
        if (x % i == 0) { d = i; break; }
    }
    if (!d) return false;             /* x is prime */
    uint64_t q = x / d;                /* remaining factor */
    /* Check that q is prime. */
    for (uint64_t i = 2; i * i <= q; ++i) {
        if (q % i == 0) return false;
    }
    return true;
}

/* Comparator for qsort that works for any seq_t. */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    return (*pa < *pb) ? -1 : ((*pa > *pb) ? 1 : 0);
}

/* Recursive generation of all permutations of subsets of {2,3,5,7}. */
static void generate(uint8_t used, seq_t val, uint8_t depth,
                     seq_t *arr, size_t *len)
{
    if (depth >= 2) {
        if (is_semiprime((uint64_t)val))
            arr[(*len)++] = val;
    }
    if (depth == 4) return;          /* maximum length reached */
    for (uint8_t i = 0; i < 4; ++i) {
        if (!(used & (1 << i))) {
            seq_t next = val * (seq_t)10 + (seq_t)digits[i];
            generate(used | (1 << i), next, depth + 1, arr, len);
        }
    }
}

/* The sequence function. */
seq_t a(idx_t n)
{
    /* Static storage for the sequence and its length. */
    static bool initialized = false;
    static seq_t seq[60];
    static size_t seq_len = 0;

    if (!initialized) {
        /* Generate all candidates. */
        generate(0, (seq_t)0, 0, seq, &seq_len);
        /* Sort in ascending order. */
        if (seq_len > 1)
            qsort(seq, seq_len, sizeof(seq_t), seq_cmp);
        initialized = true;
    }

    if (n == 0 || (idx_t)seq_len < n)
        return (seq_t)0;              /* out‑of‑range requests return 0 */

    return seq[n - 1];
}
