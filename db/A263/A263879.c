/*
 * A263879  Length k of the longest chain of primes p_1, p_2, ..., p_k such
 *          that p_1 is the n-th prime and p_{i+1} equals 2*p_i + 1 or 2*p_i -
 *          1 for all i < k, the +/- sign depending on i.
 * https://oeis.org/A263879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 6490, u32 n <= 6490, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes OEIS A263879: length of the longest chain of primes
 * p1, p2, …, pk such that p1 is the n‑th prime and
 * p_{i+1} = 2*p_i ± 1 for i < k.  The ± signs can switch only
 * when p_i is ≤ 3; otherwise the chain must follow a single
 * sign (Cunningham chain of the first or second kind).
 * The function returns the maximum possible k.
 */

#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

static seq_t chain_len_from_prime(seq_t p)
{
    /* if p ≤ 3, we may try both signs at the next step */
    if (p <= (seq_t)3) {
        seq_t best = 1;               /* at least the starting prime */

        /* try the +1 direction */
        seq_t nxt = p * 2 + 1;
        if (is_prime_big(nxt)) {
            seq_t len = chain_len_from_prime(nxt);
            if (len + 1 > best) best = len + 1;
        }

        /* try the –1 direction */
        nxt = p * 2 - 1;
        if (is_prime_big(nxt)) {
            seq_t len = chain_len_from_prime(nxt);
            if (len + 1 > best) best = len + 1;
        }

        return best;
    }

    /* for p > 3 the chain must keep a single sign */
    seq_t plus_len = 1;
    seq_t cur = p;
    while (1) {
        seq_t nxt = cur * 2 + 1;
        if (!is_prime_big(nxt)) break;
        plus_len++;
        cur = nxt;
    }

    seq_t minus_len = 1;
    cur = p;
    while (1) {
        seq_t nxt = cur * 2 - 1;
        if (!is_prime_big(nxt)) break;
        minus_len++;
        cur = nxt;
    }

    return (plus_len > minus_len) ? plus_len : minus_len;
}

seq_t a(idx_t n)
{
    /* get the n‑th prime (1‑based) */
    uint32_t prime32 = get_prime(n);
    seq_t p = (seq_t)prime32;

    return chain_len_from_prime(p);
}
