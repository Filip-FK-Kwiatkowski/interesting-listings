package org.example

import java.io.File

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
