package org.example

import java.io.File

class PlayerRecord(
    private val nameAndSurname: String,
    private val scores: List<Int?> = listOf(),
) : Comparable<PlayerRecord> {
    private val totalPoints: Int = scores.filterNotNull().sum()

    override fun compareTo(other: PlayerRecord): Int {
        return other.totalPoints.compareTo(totalPoints)
    }

    override fun toString(): String {
        val formattedPoints = scores.joinToString(separator = " | ") { it?.toString().orEmpty() }

        return "| $nameAndSurname | $$ $totalPoints $$ | $formattedPoints |"
    }

    companion object {
        fun fromLine(line: String): PlayerRecord {
            val rawPoints = line.split("|").drop(4).dropLast(1)

            val nameAndSurname = line.split("|")[2].trim()
            val points = rawPoints.map { it.trim().toIntOrNull() }

            val result = PlayerRecord(nameAndSurname, points.toMutableList())
            return result
        }
    }
}

fun main() {
    val path = "path/to/your/file.md"

    File(path)
        .walk()
        .maxDepth(1)
        .filter { it.isFile }
        .filter { it.extension == "md" && concernsSeason(it, season = 0) }
        .flatMap { extractPlayersAndDecksFrom(it) }
        .groupBy(Pair<String, *>::first, Pair<*, String>::second)
        .mapValues { (_, decks) -> decks.groupingBy { it }.eachCount() }
        .forEach { (player, decks) -> println("$player: $decks") }

}

fun extractPlayersAndDecksFrom(it: File) = it.useLines { lines ->
    fun isPlayerResultRow(line: String) = line.startsWith("| $$ ")

    lines
        .filter(::isPlayerResultRow)
        .map { it.split("|").let { columns -> columns[2] to columns[columns.lastIndex - 1]} }
        .map { (name, deck) -> name.trim() to deck.trim() }
        .toList()
}

private fun concernsSeason(it: File, season: Int) = it.name.contains("Legacy-League-$season")
