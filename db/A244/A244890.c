/*
 * A244890  "Stringed numbers": see Comments for definition.
 * https://oeis.org/A244890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 896, u32 n <= 9696, u64 n <= 9696,
 *            u128 n <= 9696, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244890: n-th stringed number via brute‑force search of Hamiltonian paths */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Convert x (idx_t) to its decimal digits.  digits[0] = most significant */
static void number_to_digits(idx_t x, uint8_t *digits, size_t *len)
{
    size_t pos = 0;
    if (x == 0)
    {
        digits[pos++] = 0;
    }
    else
    {
        while (x > 0)
        {
            digits[pos++] = (uint8_t)(x % 10);
            x /= 10;
        }
        /* reverse to get most significant first */
        for (size_t i = 0; i < pos / 2; ++i)
        {
            uint8_t tmp = digits[i];
            digits[i] = digits[pos - 1 - i];
            digits[pos - 1 - i] = tmp;
        }
    }
    *len = pos;
}

/* Return the index of the least significant set bit in mask (mask != 0) */
static int lowbit_index(unsigned int mask)
{
    int idx = 0;
    while ((mask & 1u) == 0)
    {
        mask >>= 1;
        ++idx;
    }
    return idx;
}

/* DFS search for a Hamiltonian path starting from node 'node'.
   visited: bitmask of visited nodes.  fullMask has all k bits set.  adj holds outgoing edges. */
static bool dfs_path(int node, unsigned int visited, unsigned int fullMask,
                     unsigned int *adj)
{
    if (visited == fullMask)
        return true;
    unsigned int next = adj[node] & ~visited;
    while (next)
    {
        unsigned int bit = next & -next;   /* least significant set bit */
        int nextNode = lowbit_index(bit);
        next ^= bit;
        if (dfs_path(nextNode, visited | (1u << nextNode), fullMask, adj))
            return true;
    }
    return false;
}

/* Check whether x is a stringed number according to the OEIS definition */
static bool is_stringed(idx_t x)
{
    uint8_t digits[20];
    size_t len;
    number_to_digits(x, digits, &len);

    if (len == 1)
        return true;   /* single digit numbers are trivially stringed */

    unsigned int adj[20] = {0};

    for (size_t i = 0; i < len; ++i)
    {
        int diff = (int)digits[i] + 1;
        int j1 = (int)i + diff;
        int j2 = (int)i - diff;

        if (j1 < (int)len)
            adj[i] |= (1u << j1);
        if (j2 >= 0 && j2 < (int)len)
            adj[i] |= (1u << j2);
    }

    unsigned int fullMask = (1u << len) - 1;

    for (int start = 0; start < (int)len; ++start)
    {
        if (dfs_path(start, 1u << start, fullMask, adj))
            return true;
    }
    return false;
}

/* External function required by the evaluation framework */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t cand = 0;; ++cand)
    {
        if (is_stringed(cand))
        {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
    }
}
