/*
 * A328161  Numbers n that are prime or whose proper divisors (greater than 1
 *          and less than n) have no consecutive divisibilities.
 * https://oeis.org/A328161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 172, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A328161.
   The sequence consists of positive integers that are prime or
   whose proper divisors greater than 1 and less than the number
   contain no consecutive pair where one divides the other. */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Test whether a number is prime.  Works for native integer types
   and for arbitrary‑precision integer classes that overload the
   arithmetic operators. */
static bool is_prime_seq(seq_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return (x == 2);
    for (seq_t i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

/* Return true if m satisfies the condition for A328161. */
static bool passes_rule(seq_t m)
{
    if (m < 2) return true;          /* 1 is included */
    if (is_prime_seq(m)) return true;

    /* Gather proper divisors (>1 and <m).  The array smalls is kept
       in ascending order by construction.  The array bigs stores
       the larger divisors in descending order.  The combined set
       of divisors is then obtained by concatenating smalls and
       bigs reversed. */
    seq_t smalls[128];
    seq_t bigs[128];
    size_t small_cnt = 0, big_cnt = 0;

    for (seq_t i = 2; i * i <= m; ++i) {
        if (m % i == 0) {
            smalls[small_cnt++] = i;
            seq_t other = m / i;
            if (other != i)          /* exclude the duplicate when m is a square */
                bigs[big_cnt++] = other;  /* stored in descending order */
        }
    }

    /* Check consecutive pairs within smalls (already sorted). */
    for (size_t j = 1; j < small_cnt; ++j) {
        seq_t a = smalls[j - 1], b = smalls[j];
        if ((a % b == 0) || (b % a == 0))
            return false;
    }

    /* Cross pair between the last small divisor and the first big
       divisor (after reversing bigs to ascending). */
    if (small_cnt > 0 && big_cnt > 0) {
        seq_t a = smalls[small_cnt - 1];
        seq_t b = bigs[big_cnt - 1];     /* ascending after reverse */
        if ((a % b == 0) || (b % a == 0))
            return false;
    }

    /* Check consecutive pairs within bigs in ascending order. */
    for (size_t k = big_cnt; k-- > 1; ) {
        seq_t a = bigs[k];
        seq_t b = bigs[k - 1];
        if ((a % b == 0) || (b % a == 0))
            return false;
    }

    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined, but shouldn't be called */

    /* Memoisation cache for speed on multiple calls. */
    static seq_t *cache = NULL;
    static idx_t cachelen = 0;
    static idx_t cachecap = 0;
    static seq_t last_checked = 0;

    if (cache == NULL) {
        cachecap = 64;
        cache = (seq_t *)malloc(cachecap * sizeof(seq_t));
        if (!cache) return 0;     /* allocation failure */
    }

    /* Generate terms until the cache holds at least n entries. */
    while (cachelen < n) {
        /* Find the next integer that satisfies the rule. */
        ++last_checked;
        while (!passes_rule(last_checked))
            ++last_checked;

        /* Resize the cache if necessary. */
        if (cachelen >= cachecap) {
            size_t newcap = cachecap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return 0;  /* allocation failure */
            cache = tmp;
            cachecap = newcap;
        }

        cache[cachelen++] = last_checked;
    }

    return cache[n - 1];
}
